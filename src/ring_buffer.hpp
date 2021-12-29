#pragma once

#include <stdlib.h>
#include <threads.h>

enum rbuf_result { rbuf_success, rbuf_error };

struct rbuf {
  size_t start;
  size_t taken;
  size_t capacity;
  unsigned char *buffer;
  mtx_t mtx;
};


rbuf_result rbuf_init(rbuf &b, size_t capacity);
void rbuf_destroy(rbuf &b);
size_t rbuf_capacity(rbuf &b);
size_t rbuf_available(rbuf &b);
size_t rbuf_taken(rbuf &b);
size_t rbuf_put(rbuf &b, const unsigned char *data, size_t size);
size_t rbuf_get(rbuf &b, unsigned char *data, size_t size);
