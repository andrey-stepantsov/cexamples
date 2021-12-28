#include "ring_buffer.hpp"
#include <gtest/gtest.h>

TEST(RingBufferTest, Initialization) {
  rbuf b;
  rbuf_init(b);
  EXPECT_EQ(b.start, 0);
  EXPECT_EQ(b.end, 0);
  EXPECT_EQ(rbuf_taken(b), 0);
  EXPECT_EQ(rbuf_available(b), rbuf_size(b));
}

TEST(RingBufferTest, PutOneChar) {
  rbuf b;
  rbuf_init(b);
  const size_t capacity = rbuf_size(b);
  EXPECT_EQ(rbuf_taken(b), 0);
  unsigned char data[] = " ";
  EXPECT_EQ(rbuf_put(b, data, 1), 1);
  EXPECT_EQ(rbuf_taken(b), 1);
  EXPECT_EQ(rbuf_available(b), capacity - rbuf_taken(b));
}

TEST(RingBufferTest, PutAndGetOneChar) {
  rbuf b;
  rbuf_init(b);
  size_t capacity = rbuf_size(b);
  unsigned char data[1] = {'a'};
  EXPECT_EQ(rbuf_put(b, data, 1), 1);
  EXPECT_EQ(rbuf_taken(b), 1);
  EXPECT_EQ(rbuf_available(b), capacity - 1);
  EXPECT_EQ(rbuf_get(b, data, 1), 1);
  EXPECT_EQ(rbuf_taken(b), 0);
  EXPECT_EQ(rbuf_available(b), capacity);
  EXPECT_EQ(data[0], 'a');
}

TEST(RingBufferTest, PutAndGetThreeChars) {
  rbuf b;
  rbuf_init(b);
  size_t capacity = rbuf_size(b);
  EXPECT_EQ(rbuf_taken(b), 0);
  unsigned char data[3] = {'a', 'b', 'c'};
  EXPECT_EQ(rbuf_put(b, data, 3), 3);
  EXPECT_EQ(rbuf_taken(b), 3);
  EXPECT_EQ(rbuf_available(b), capacity - 3);
  EXPECT_EQ(rbuf_get(b, data, 3), 3);
  EXPECT_EQ(rbuf_taken(b), 0);
  EXPECT_EQ(rbuf_available(b), capacity);
  EXPECT_EQ(data[0], 'a');
  EXPECT_EQ(data[1], 'b');
  EXPECT_EQ(data[2], 'c');
}

TEST(RingBufferTest, PutOneLessThanCapacity) {
  rbuf b;
  rbuf_init(b);
  const size_t to_put = rbuf_size(b) - 1;
  EXPECT_EQ(rbuf_taken(b), 0);
  unsigned char data[to_put];
  EXPECT_EQ(rbuf_put(b, data, to_put), to_put);
  EXPECT_EQ(rbuf_taken(b), to_put);
}

TEST(RingBufferTest, PutOneMoreThanCapacity) {
  rbuf b;
  rbuf_init(b);
  size_t to_put = rbuf_size(b) + 1;
  EXPECT_EQ(rbuf_taken(b), 0);
  unsigned char data[to_put];
  EXPECT_EQ(rbuf_put(b, data, to_put), to_put - 1);
}

TEST(RingBufferTest, Complex) {
  rbuf b;
  rbuf_init(b);
  const size_t capacity = rbuf_size(b);
  unsigned char data[capacity - 1];

  EXPECT_EQ(rbuf_put(b, data, capacity - 1), capacity - 1);
  EXPECT_EQ(rbuf_taken(b), capacity - 1);
  EXPECT_EQ(rbuf_get(b, data, capacity - 1), capacity - 1);

  EXPECT_EQ(rbuf_put(b, data, capacity - 1), capacity - 1);
  EXPECT_EQ(rbuf_taken(b), capacity - 1);
  EXPECT_EQ(rbuf_get(b, data, capacity - 1), capacity - 1);

  EXPECT_EQ(rbuf_put(b, data, capacity - 1), capacity - 1);
  EXPECT_EQ(rbuf_taken(b), capacity - 1);
  EXPECT_EQ(rbuf_get(b, data, capacity - 1), capacity - 1);

  EXPECT_EQ(rbuf_put(b, data, capacity - 1), capacity - 1);
  EXPECT_EQ(rbuf_taken(b), capacity - 1);
  EXPECT_EQ(rbuf_get(b, data, capacity - 1), capacity - 1);

  unsigned char test_data[5] = {'a', 'b', 'c', 'd', 'e'};

  EXPECT_EQ(rbuf_put(b, test_data, 5), 5);
  EXPECT_EQ(rbuf_taken(b), 5);
  EXPECT_EQ(rbuf_get(b, test_data, 5), 5);
  EXPECT_EQ(rbuf_taken(b), 0);
  EXPECT_EQ(test_data[0], 'a');
  EXPECT_EQ(test_data[1], 'b');
  EXPECT_EQ(test_data[2], 'c');
  EXPECT_EQ(test_data[3], 'd');
  EXPECT_EQ(test_data[4], 'e');
}
