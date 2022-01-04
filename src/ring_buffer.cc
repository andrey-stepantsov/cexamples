#include "ring_buffer.hpp"
#include <stdlib.h>

/**
 * Initializes the ring buffer control structure by allocating memory and
 * initializing the mutex supporting thread safety.
 *
 * @param[out] b the control structure of a ring buffer to initialize
 * @param[in] capacity of the ring buffer, allocated from the dynamic memory
 */
rbuf_result rbuf_init(rbuf &b, size_t capacity) {
    b.start = 0;
    b.buffer = (unsigned char *)malloc(capacity);
    b.taken = 0;
    b.capacity = capacity;
    pthread_mutexattr_t attr;
    rbuf_result result = rbuf_error;
    if (0 == pthread_mutexattr_init(&attr)) {
        if (0 == pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE)) {
            if (0 == pthread_mutex_init(&b.mtx, &attr)) {
                b.buffer = (unsigned char *)malloc(capacity);
                if (NULL != b.buffer)
                    result = rbuf_success;
                else
                    pthread_mutex_destroy(&b.mtx);
            }
        }
        pthread_mutexattr_destroy(&attr);
    }
    return result;
}

/**
 * Releases resources held by the ring buffer: dynamic memory and the mutex used
 * to support thread safety
 * @param[in] b - the buffer to destroy
 */
void rbuf_destroy(rbuf &b) {
    free(b.buffer);
    // mtx_destroy(&b.mtx);
    pthread_mutex_destroy(&b.mtx);
}

/**
 * Returns a ring buffers capacity - total amount of data that can be sotred by
 * the buffer
 * @param[in] b - the buffer to report capacity of
 * @result capacity of the ring buffer, or -1 on failure, i.e. lock error
 */
size_t rbuf_capacity(rbuf &b) {
    if (pthread_mutex_lock(&b.mtx))
        return -1;
    const size_t capacity = b.capacity;
    if (pthread_mutex_unlock(&b.mtx))
        return -1;
    return capacity;
};

/**
 * Retruns size of data currently held by a ring buffer
 * @param[in] b - the buffer to report on
 * @result size of data in the ring buffer, or -1 on failure, i.e. lock error
 */
size_t rbuf_taken(rbuf &b) {
    if (pthread_mutex_lock(&b.mtx))
        return -1;
    const size_t taken = b.taken;
    if (pthread_mutex_unlock(&b.mtx))
        return -1;
    return taken;
}

/**
 * Reports size of free space in the ring buffer
 * @param[in] b - the buffer to report on
 * @result available space, or -1 on failure, i.e. lock error
 */
size_t rbuf_available(rbuf &b) {
    if (pthread_mutex_lock(&b.mtx))
        return -1;
    const size_t available = b.capacity - b.taken;
    if (pthread_mutex_unlock(&b.mtx))
        return -1;
    return available;
}

/**
 * Puts data into a ring buffer. The actual amount of the data placed into the
 * ring buffer could be less than being passed to the function
 * @param[out] b the ring buffer to put the data into
 * @param[in] data to be put into the ring buffer
 * @param[in] size of the data to be put into the ring buffer
 * @result actual size of the data successully put, or -1 on failure
 */
size_t rbuf_put(rbuf &b, const unsigned char *data, size_t size) {
    if (pthread_mutex_lock(&b.mtx))
        return -1;
    const size_t available = rbuf_available(b);
    const size_t to_copy = size > available ? available : size;
    for (size_t i = 0; i < to_copy; ++i) {
        b.buffer[(b.start + b.taken + i) % b.capacity] = *(data + i);
    }
    b.taken += to_copy;
    if (pthread_mutex_unlock(&b.mtx))
        return -1;
    return to_copy;
}

/**
 * Gets data from a ring buffer. The actual size of the data read from the
 * buffer could be less than defined by the size parameter.
 * @param[out] b the buffer to get data from
 * @param[out] data buffer to get the data into
 * @param[in] size that can be held by the data buffer
 * @result actual size of the data successully read, or -1 on failure
 */
size_t rbuf_get(rbuf &b, unsigned char *data, size_t size) {
    if (pthread_mutex_lock(&b.mtx))
        return -1;
    const size_t taken = rbuf_taken(b);
    const size_t to_copy = size > taken ? taken : size;
    for (size_t i = 0; i < to_copy; ++i) {
        *(data + i) = b.buffer[(b.start + i) % b.capacity];
    }
    (b.start += to_copy) %= b.capacity;
    b.taken -= to_copy;
    if (pthread_mutex_unlock(&b.mtx))
        return -1;
    return to_copy;
}
