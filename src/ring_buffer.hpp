#pragma once

#include <stdlib.h>
#include <pthread.h>

/**
 * @page ring_buffer Ring Buffer
 * @ref time_util
 */

/**
 * enum to indicate result of a buffer initialization call
 */
enum rbuf_result {
    /**
     * indicates success of @ref rbuf_init
     */
    rbuf_success,
    /**
     * indicates failure of @ref rbuf_init
     */
    rbuf_error
};

/**
 * @brief the ring buffer control
 * 
 */
struct rbuf {
    size_t start;
    size_t taken;
    size_t capacity;
    unsigned char *buffer;
    pthread_mutex_t mtx;
};

rbuf_result rbuf_init(rbuf &b, size_t capacity);
void rbuf_destroy(rbuf &b);
size_t rbuf_capacity(rbuf &b);
size_t rbuf_available(rbuf &b);
size_t rbuf_taken(rbuf &b);
size_t rbuf_put(rbuf &b, const unsigned char *data, size_t size);
size_t rbuf_get(rbuf &b, unsigned char *data, size_t size);
