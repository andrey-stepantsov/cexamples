#include "ring_buffer.hpp"
#include <stdlib.h>

rbuf_result rbuf_init(rbuf &b, size_t capacity) {
  b.start = 0;
  b.buffer = (unsigned char *)malloc(capacity);
  b.taken = 0;
  if (NULL == b.buffer)
    return rbuf_error;
  b.capacity = capacity;
  if (thrd_success != mtx_init(&b.mtx, mtx_recursive)) {
    free(b.buffer);
    return rbuf_error;
  }
  return rbuf_success;
}

void rbuf_destroy(rbuf &b) {
  free(b.buffer);
  mtx_destroy(&b.mtx);
}

size_t rbuf_capacity(rbuf &b) {
  if (thrd_success != mtx_lock(&b.mtx))
    return -1;
  const size_t capacity = b.capacity;
  if (thrd_success != mtx_unlock(&b.mtx))
    return -1;
  return capacity;
};

size_t rbuf_taken(rbuf &b) {
  if (thrd_success != mtx_lock(&b.mtx))
    return -1;
  const size_t taken = b.taken;
  if (thrd_success != mtx_unlock(&b.mtx))
    return -1;
  return taken;
}

size_t rbuf_available(rbuf &b) {
  if (thrd_success != mtx_lock(&b.mtx))
    return -1;
  const size_t available = b.capacity - b.taken;
  if (thrd_success != mtx_unlock(&b.mtx))
    return -1;
  return available;
}

size_t rbuf_put(rbuf &b, const unsigned char *data, size_t size) {
  if (thrd_success != mtx_lock(&b.mtx))
    return -1;
  const size_t available = rbuf_available(b);
  const size_t to_copy = size > available ? available : size;
  for (size_t i = 0; i < to_copy; ++i) {
    b.buffer[(b.start + b.taken + i) % b.capacity] = *(data + i);
  }
  b.taken += to_copy;
  if (thrd_success != mtx_unlock(&b.mtx))
    return -1;
  return to_copy;
}

size_t rbuf_get(rbuf &b, unsigned char *data, size_t size) {
  if (thrd_success != mtx_lock(&b.mtx))
    return -1;
  const size_t taken = rbuf_taken(b);
  const size_t to_copy = size > taken ? taken : size;
  for (size_t i = 0; i < to_copy; ++i) {
    *(data + i) = b.buffer[(b.start + i) % b.capacity];
  }
  (b.start += to_copy) %= b.capacity;
  b.taken -= to_copy;
  if (thrd_success != mtx_unlock(&b.mtx))
    return -1;
  return to_copy;
}
