#include "ring_buffer.hpp"

rbuf_result rbuf_init(rbuf &b) {
  b.start = 0;
  b.end = 0;
  return thrd_success == mtx_init(&b.mtx, mtx_recursive) ? rbuf_success
                                                     : rbuf_error;
}

void rbuf_destroy(rbuf &b) { mtx_destroy(&b.mtx); }

size_t rbuf_size(rbuf &b) {
  if (thrd_success != mtx_lock(&b.mtx))
    return -1;
  const size_t size = sizeof(b.buffer);
  if (thrd_success != mtx_unlock(&b.mtx))
    return -1;
  return size;
};

size_t rbuf_taken(rbuf &b) {
  if (thrd_success != mtx_lock(&b.mtx))
    return -1;
  const size_t taken =
      b.end >= b.start ? b.end - b.start : b.end + buf_size - b.start;
  if (thrd_success != mtx_unlock(&b.mtx))
    return -1;
  return taken;
}

size_t rbuf_available(rbuf &b) { return buf_size - rbuf_taken(b); }

size_t rbuf_put(rbuf &b, const unsigned char *data, size_t size) {
  if (thrd_success != mtx_lock(&b.mtx))
    return -1;
  const size_t available = rbuf_available(b);
  const size_t to_copy = size > available ? available : size;
  for (size_t i = 0; i < to_copy; ++i) {
    b.buffer[(b.end + i) % buf_size] = *(data + i);
  }
  (b.end += to_copy) %= buf_size;
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
    *(data + i) = b.buffer[(b.start + i) % buf_size];
  }
  (b.start += to_copy) %= buf_size;
  if (thrd_success != mtx_unlock(&b.mtx))
    return -1;
  return to_copy;
}
