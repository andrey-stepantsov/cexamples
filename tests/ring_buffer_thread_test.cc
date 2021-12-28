#include "ring_buffer.hpp"
#include <gtest/gtest.h>
#include <stdio.h>
#include <strings.h>
#include <threads.h>

int put_three(void *arg) {
  rbuf *b = (rbuf *)arg;
  const unsigned char data[3] = {'a', 'b', 'c'};
  rbuf_put(*b, data, 3);
  return 0;
}

TEST(RingBufferThreadTest, PutThree) {
  rbuf b;
  rbuf_init(b);
  EXPECT_EQ(b.start, 0);
  EXPECT_EQ(b.end, 0);
  EXPECT_EQ(rbuf_taken(b), 0);
  EXPECT_EQ(rbuf_available(b), rbuf_size(b));
  thrd_t thrd;
  if (thrd_success == thrd_create(&thrd, put_three, &b)) {
    int result;
    thrd_join(thrd, &result);
    EXPECT_EQ(rbuf_taken(b), 3);
  } else {
    FAIL();
  }
}

const timespec pump_sleep_duration{.tv_nsec = 100};
const timespec drain_sleep_duration{.tv_nsec = 10000};
const int max_rep_count = 10000;
const int max_put_count = 1000;

int pump_data(void *arg) {
  rbuf *b = (rbuf *)arg;
  const int repetitions = 17000;
  size_t pumped = 0;
  int count = 0;
  while (count <= repetitions && count < max_rep_count) {
    ++count;
    char buffer[1024];
    sprintf(buffer, "%.5d;", count);
    size_t size = strlen(buffer);
    size_t offset = 0;
    int put_count = 0;
    while (size != offset && put_count < max_put_count) {
      ++put_count;
      offset += rbuf_put(*b, (unsigned char *)buffer + offset, size - offset);
      thrd_sleep(&pump_sleep_duration, NULL);
    }
    pumped += offset;
  }
  return pumped;
}

int drain_data(void *arg) {
  rbuf *b = (rbuf *)arg;
  size_t drained = 0;
  int count = 0;
  int zero_read = 0;
  while (count < max_rep_count) {
    ++count;
    const size_t size = 256;
    char buffer[size];
    size_t read = rbuf_get(*b, (unsigned char *)buffer, size);
    if (0 == read) {
      zero_read++;
    }
    drained += read;
    thrd_sleep(&drain_sleep_duration, NULL);
  }
  printf("zero reads: %d\n", zero_read);
  return drained;
}

TEST(RingBufferThreadTest, PumpData) {
  rbuf b;
  rbuf_init(b);
  thrd_t pump_thrd;
  thrd_t drain_thrd;
  if (thrd_success == thrd_create(&pump_thrd, pump_data, &b) &&
      thrd_success == thrd_create(&drain_thrd, drain_data, &b)) {
    int pump_result;
    int drain_result;
    thrd_join(pump_thrd, &pump_result);
    printf("pumped: %d\n", pump_result);
    thrd_join(drain_thrd, &drain_result);
    printf("drained: %d\n", drain_result);
    EXPECT_EQ(pump_result, drain_result);
  } else {
    FAIL();
  }
}
