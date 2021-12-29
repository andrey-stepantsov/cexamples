#include "ring_buffer.hpp"
#include "time_util.hpp"
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
  rbuf_init(b, 1024);
  EXPECT_EQ(b.start, 0);
  EXPECT_EQ(rbuf_taken(b), 0);
  EXPECT_EQ(rbuf_available(b), rbuf_capacity(b));
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
const timespec drain_sleep_duration{.tv_nsec = 1000};
const long int max_put_count = 10000;

int pump_data(void *arg) {
  rbuf *b = (rbuf *)arg;
  const double start = wall_time();
  const double stop = start + 1.0;
  size_t pumped = 0;
  int count = 0;
  char buffer[500];
  while (1) {
    ++count;
//    sprintf(buffer, "%.5d;", count);
    size_t size = sizeof(buffer);
//	size_t size = strlen(buffer);
    size_t offset = 0;
    int put_count = 0;
    while (size != offset && put_count < max_put_count) {
      ++put_count;
      offset += rbuf_put(*b, (unsigned char *)buffer + offset, size - offset);
      thrd_sleep(&pump_sleep_duration, NULL);
    }
    pumped += offset;
    const double now = wall_time();
    if (now > stop)
      break;
  }
  return pumped;
}

int drain_data(void *arg) {
  rbuf *b = (rbuf *)arg;
  size_t drained = 0;
  const double start = wall_time();
  const double stop = start + 2.0;
  int count = 0;
  int zero_read = 0;
  const size_t size = 256;
  char buffer[size];
  while (1) {
    ++count;
    size_t read = rbuf_get(*b, (unsigned char *)buffer, size);
    if (0 == read) {
      zero_read++;
    }
    drained += read;
    thrd_sleep(&drain_sleep_duration, NULL);
    const double now = wall_time();
    if (now > stop)
      break;
  }
  printf("zero reads: %d\n", zero_read);
  return drained;
}

TEST(RingBufferThreadTest, PumpData) {
  rbuf b;
  rbuf_init(b, 1024);
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
  rbuf_destroy(b);
}
