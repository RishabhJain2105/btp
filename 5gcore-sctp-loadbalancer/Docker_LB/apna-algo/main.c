#include <arpa/inet.h>
#include <netinet/sctp.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

#include "amf.h"
#include "forward.h"
#include "scale.h"
#include "utils.h"

int main(int argc, char *argv[]) {
    (void)argc;
    (void)argv;

    {
        struct stat st = {0};
        if (stat("logs", &st) == -1) {
            if (mkdir("logs", 0755) != 0) {
                perror("[main] mkdir logs");
            }
        }

        time_t t = time(NULL);
        struct tm tm = *localtime(&t);
        char fname[256];
        if (strftime(fname, sizeof(fname), "logs/run-%Y%m%d-%H%M%S.log", &tm) == 0) {
            snprintf(fname, sizeof(fname), "logs/run-%ld.log", (long)t);
        }

        if (log_init(fname) != 0) {
            fprintf(stderr, "[main] Failed to initialize log file %s\n", fname);
        } else {
            log("INFO", "[main] Logging initialized at %s\n", fname);
        }
    }

    log("INFO", "[main] Initializing AMF and forwarding tables...\n"); // Line 16
    amf_init_default();
    forward_init_table();

    int listen_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_SCTP);
    if (listen_socket < 0) {
        log_perror("[main] socket");
        return 1;
    }

    struct sockaddr_in listen_addr;
    memset(&listen_addr, 0, sizeof(listen_addr));
    listen_addr.sin_family = AF_INET;
    listen_addr.sin_addr.s_addr = inet_addr("10.0.3.1");
    listen_addr.sin_port = htons(38412);
    
    log("INFO", "[main] Binding socket to 10.0.3.1:38412\n"); 
    if (bind(listen_socket, (struct sockaddr *)&listen_addr, sizeof(listen_addr)) < 0) {
        log_perror("[main] bind");
        close(listen_socket);
        return 1;
    }
    
    log("INFO", "[main] Setting up socket for listening (%d max connections)...\n", MAX_CONNECTIONS);
    if (listen(listen_socket, MAX_CONNECTIONS) < 0) {
        log_perror("[main] listen");
        close(listen_socket);
        return 1;
    }

    log("INFO", "[main] Proxy listening on 10.0.3.1:38412 with AMF capacity %d\n", AMF_CAPACITY);

    pthread_t descaling_t;
    if (pthread_create(&descaling_t, NULL, descaling_thread_func, NULL) != 0) {
        log_perror("[main:51] pthread_create");
        close(listen_socket);
        return 1;
    }
    pthread_detach(descaling_t);

    while (1) {
        struct sockaddr_in gnb_addr;
        socklen_t addr_len = sizeof(gnb_addr);
        int *gnb_sock = malloc(sizeof(int));
        if (!gnb_sock) {
            log_perror("[main] malloc");
            continue;
        }
        *gnb_sock = accept(listen_socket, (struct sockaddr *)&gnb_addr, &addr_len);

        if (*gnb_sock < 0) {
            log_perror("[main] accept failed");
            free(gnb_sock);
            continue;
        }
        log("INFO", "[main] Accepted new GNB connection from %s:%d\n", inet_ntoa(gnb_addr.sin_addr), ntohs(gnb_addr.sin_port)); 

        scale_up_check();

        pthread_t gnb_thread;
        if (pthread_create(&gnb_thread, NULL, handle_gnb_connection, gnb_sock) != 0) {
            log_perror("[main:79] pthread_create");
            close(*gnb_sock);
            free(gnb_sock);
            continue;
        }
        pthread_detach(gnb_thread);
    }
    log("INFO", "[main] Closing listening socket and exiting.\n");
    log_close();

    close(listen_socket);
    return 0;
}