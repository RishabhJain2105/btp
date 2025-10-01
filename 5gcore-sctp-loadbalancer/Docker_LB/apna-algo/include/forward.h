#ifndef FORWARD_H
#define FORWARD_H

#include <pthread.h>

#include "amf.h"

#define BUFFER_SIZE 1024

typedef struct {
    int source_socket;
    int *destination_socket;  // pointer to fd to allow live replacement
    AMF **current_amf;        // pointer to pointer for live migration
    int live_thread_index;    // index in the live_threads table
    int is_active;
} forward_info_t;

void forward_init_table(void);
int forward_register_thread(forward_info_t *info);
void forward_unregister_index(int idx);
void *handle_gnb_connection(void *arg);

extern forward_info_t *live_threads[MAX_CONNECTIONS];
extern pthread_mutex_t live_threads_mutex;

#endif  // FORWARD_H
