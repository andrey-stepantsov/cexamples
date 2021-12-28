#pragma once

#include <stdlib.h>
#include <threads.h>

const size_t buf_size = 1024*1;

enum rbuf_result { rbuf_success, rbuf_error };

struct rbuf {
  unsigned char buffer[buf_size];
  size_t start;
  size_t end;
  mtx_t mtx;
};


rbuf_result rbuf_init(rbuf &b);
void rbuf_destory(rbuf &b);
size_t rbuf_size(rbuf &b);
size_t rbuf_available(rbuf &b);
size_t rbuf_taken(rbuf &b);
size_t rbuf_put(rbuf &b, const unsigned char *data, size_t size);
size_t rbuf_get(rbuf &b, unsigned char *data, size_t size);
