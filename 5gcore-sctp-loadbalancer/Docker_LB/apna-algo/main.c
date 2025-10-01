#include <arpa/inet.h>
#include <netinet/sctp.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "amf.h"
#include "forward.h"
#include "scale.h"

int main(int argc, char *argv[]) {
    (void)argc;
    (void)argv;

    printf("[main] Initializing AMF and forwarding tables...\n"); // Line 16
    amf_init_default();
    forward_init_table();

    int listen_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_SCTP);
    if (listen_socket < 0) {
        perror("[main] socket");
        return 1;
    }

    struct sockaddr_in listen_addr;
    memset(&listen_addr, 0, sizeof(listen_addr));
    listen_addr.sin_family = AF_INET;
    listen_addr.sin_addr.s_addr = inet_addr("10.0.3.1");
    listen_addr.sin_port = htons(38412);
    
    printf("[main] Binding socket to 10.0.3.1:38412\n"); 
    if (bind(listen_socket, (struct sockaddr *)&listen_addr, sizeof(listen_addr)) < 0) {
    perror("[main] bind");
        close(listen_socket);
        return 1;
    }
    
    printf("[main] Setting up socket for listening (%d max connections)...\n", MAX_CONNECTIONS);
    if (listen(listen_socket, MAX_CONNECTIONS) < 0) {
        perror("[main] listen");
        close(listen_socket);
        return 1;
    }

    printf("[main] Proxy listening on 10.0.3.1:38412 with AMF capacity %d\n", AMF_CAPACITY);

    pthread_t descaling_t;
    if (pthread_create(&descaling_t, NULL, descaling_thread_func, NULL) != 0) {
        perror("[main:51] pthread_create");
        close(listen_socket);
        return 1;
    }
    pthread_detach(descaling_t);

    while (1) {
        struct sockaddr_in gnb_addr;
        socklen_t addr_len = sizeof(gnb_addr);
        int *gnb_sock = malloc(sizeof(int));
        if (!gnb_sock) {
            perror("[main] malloc");
            continue;
        }
        *gnb_sock = accept(listen_socket, (struct sockaddr *)&gnb_addr, &addr_len);

        if (*gnb_sock < 0) {
            perror("[main] accept failed");
            free(gnb_sock);
            continue;
        }
        printf("[main] Accepted new GNB connection from %s:%d\n", inet_ntoa(gnb_addr.sin_addr), ntohs(gnb_addr.sin_port)); 

        scale_up_check();
        printf("[main] Scale up check completed.\n");

        pthread_t gnb_thread;
        if (pthread_create(&gnb_thread, NULL, handle_gnb_connection, gnb_sock) != 0) {
            perror("[main:79] pthread_create");
            close(*gnb_sock);
            free(gnb_sock);
            continue;
        }
        pthread_detach(gnb_thread);
    }
    printf("[main] Closing listening socket and exiting.\n");
    close(listen_socket);
    return 0;
}