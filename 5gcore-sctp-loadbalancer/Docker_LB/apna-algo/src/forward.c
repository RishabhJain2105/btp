#include "forward.h"

#include <errno.h>
#include <netinet/sctp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include "amf.h"
#include "scale.h"
#include "utils.h"

pthread_mutex_t live_threads_mutex = PTHREAD_MUTEX_INITIALIZER;
forward_info_t *live_threads[MAX_CONNECTIONS];

extern pthread_mutex_t amf_state_mutex;  // declared in scaling.c (shared state)
extern int total_conn_count;             // declared in scaling.c

void forward_init_table(void) {
    printf("[forward] forward_init_table: Initializing live thread table\n");
    pthread_mutex_lock(&live_threads_mutex);
    for (int i = 0; i < MAX_CONNECTIONS; i++) live_threads[i] = NULL;
    pthread_mutex_unlock(&live_threads_mutex);
}

static void cleanup_forward_info(forward_info_t *info) {
    if (!info) return;
    printf("[forward] cleanup_forward_info: Closing source socket %d\n", info->source_socket);
    close(info->source_socket);
    if (info->destination_socket && *(info->destination_socket) > 0) {
        printf("[forward] cleanup_forward_info: Closing destination socket %d\n", *(info->destination_socket));
        close(*(info->destination_socket));
    }
    free(info->destination_socket);
    free(info->current_amf);
    free(info);
}

void *forward_messages(void *arg) {
    forward_info_t *info = (forward_info_t *)arg;
    char buffer[BUFFER_SIZE];
    ssize_t nbytes;

    printf("[forward] forward_messages: Started forwarding messages (thread index %d)\n", info->live_thread_index);

    while ((nbytes = sctp_recvmsg(info->source_socket, buffer, sizeof(buffer), NULL, 0, NULL, NULL)) > 0) {
        int dest_sock = -1;
        AMF *temp_amf = *(info->current_amf);

        // Lock AMF to safely read the socket descriptor if needed
        pthread_mutex_lock(&temp_amf->lock);
        dest_sock = *(info->destination_socket);
        pthread_mutex_unlock(&temp_amf->lock);

        if (dest_sock > 0) {
            ssize_t sent = sctp_sendmsg(dest_sock, buffer, nbytes, NULL, 0, 0, 0, 0, 0, 0);
            if (sent < 0) {
                perror("[forward] forward_messages: sctp_sendmsg error");
            }
        }
        printf("[forward] forward_messages: Transfered %d bytes of data; sockets %d->%d\n",nbytes, info->source_socket, dest_sock);

    }
    printf("[forward] forward_messages: Connection closed or error on source socket %d\n", info->source_socket);

    // Cleanup connection counts
    pthread_mutex_lock(&amf_state_mutex);
    if (*info->current_amf) {
        (*info->current_amf)->connections--;
        total_conn_count--;
        printf("[forward] forward_messages: Decremented connection counts for AMF id=%d, total_conn_count=%d\n",
               (*info->current_amf)->id, total_conn_count);
    }
    pthread_mutex_unlock(&amf_state_mutex);

    // Unregister from live table
    pthread_mutex_lock(&live_threads_mutex);
    info->is_active = 0;
    if (info->live_thread_index >= 0 && info->live_thread_index < MAX_CONNECTIONS) {
        live_threads[info->live_thread_index] = NULL;
        printf("[forward] forward_messages: Unregistered thread index %d\n", info->live_thread_index);
    }
    pthread_mutex_unlock(&live_threads_mutex);

    cleanup_forward_info(info);
    return NULL;
}

int forward_register_thread(forward_info_t *info) {
    pthread_mutex_lock(&live_threads_mutex);
    int found_slot = -1;
    for (int i = 0; i < MAX_CONNECTIONS; i++) {
        if (live_threads[i] == NULL) {
            live_threads[i] = info;
            info->live_thread_index = i;
            found_slot = i;
            printf("[forward] forward_register_thread: Registered thread at slot %d\n", i);
            break;
        }
    }
    pthread_mutex_unlock(&live_threads_mutex);
    if (found_slot == -1) {
        printf("[forward] forward_register_thread: No free slot available\n");
    }
    return found_slot;
}

void forward_unregister_index(int idx) {
    if (idx < 0 || idx >= MAX_CONNECTIONS) {
        printf("[forward] forward_unregister_index: Invalid index %d\n", idx);
        return;
    }
    pthread_mutex_lock(&live_threads_mutex);
    if (live_threads[idx]) {
        live_threads[idx]->is_active = 0;
        live_threads[idx] = NULL;
        printf("[forward] forward_unregister_index: Unregistered thread at index %d\n", idx);
    }
    pthread_mutex_unlock(&live_threads_mutex);
}

void *handle_gnb_connection(void *arg) {
    int gnb_socket = *(int *)arg;
    free(arg);

    printf("[forward] handle_gnb_connection: Handling new gNB socket %d\n", gnb_socket);
    AMF *target_amf = get_next_amf_round_robin();
    int amf_sock;
    do {
        if (!target_amf) {
            printf("[forward] handle_gnb_connection: No active AMF available, closing gNB socket %d\n", gnb_socket);
            close(gnb_socket);
            return NULL;
        }

        extern pthread_mutex_t amf_state_mutex;

        // Consistent locking order: global then specific
        pthread_mutex_lock(&amf_state_mutex);
        pthread_mutex_lock(&target_amf->lock);

        int amf_sock = connect_to_amf(target_amf);
        if (amf_sock < 0) {
            printf("[forward] handle_gnb_connection: Failed to connect to AMF id=%d ip=%s\n", target_amf->id, target_amf->ip);
            pthread_mutex_unlock(&target_amf->lock);
            pthread_mutex_unlock(&amf_state_mutex);
            // close(gnb_socket);
            // return NULL;
        }
    } while (amf_sock < 0);
    
    target_amf->connections++;
    extern int total_conn_count;
    total_conn_count++;
    printf("[forward] handle_gnb_connection: Connected to AMF id=%d, incremented connections to %d; total_conn_count=%d\n",
           target_amf->id, target_amf->connections, total_conn_count);

    pthread_mutex_unlock(&target_amf->lock);
    pthread_mutex_unlock(&amf_state_mutex);

    // Allocate structures for forwarding thread
    forward_info_t *gnb_to_amf = malloc(sizeof(forward_info_t));
    if (!gnb_to_amf) {
        printf("[forward] handle_gnb_connection: Memory allocation failed for forward_info_t\n");
        close(gnb_socket);
        close(amf_sock);
        return NULL;
    }

    gnb_to_amf->source_socket = gnb_socket;
    gnb_to_amf->destination_socket = malloc(sizeof(int));
    *(gnb_to_amf->destination_socket) = amf_sock;
    gnb_to_amf->current_amf = malloc(sizeof(AMF *));
    *(gnb_to_amf->current_amf) = target_amf;
    gnb_to_amf->is_active = 1;
    gnb_to_amf->live_thread_index = -1;

    int slot = forward_register_thread(gnb_to_amf);
    if (slot == -1) {
        fprintf(stderr, "[forward] handle_gnb_connection: Max connections reached. Cannot handle new connection.\n");
        cleanup_forward_info(gnb_to_amf);
        return NULL;
    }

    pthread_t thread;
    if (pthread_create(&thread, NULL, forward_messages, gnb_to_amf) != 0) {
        perror("[forward] handle_gnb_connection: pthread_create error");
        forward_unregister_index(slot);
        cleanup_forward_info(gnb_to_amf);
        return NULL;
    }
    pthread_detach(thread);

    printf("[forward] handle_gnb_connection: Forwarding thread started at index %d\n", slot);
    return NULL;
}
