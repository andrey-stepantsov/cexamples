#include "ring_buffer.hpp"
#include "time_util.hpp"
#include <gtest/gtest.h>
#include <stdio.h>
#include <strings.h>
#include <pthread.h>

void* put_three(void *arg) {
    int *result = (int*)malloc(sizeof(int));
    rbuf *b = (rbuf *)arg;
    const unsigned char data[3] = {'a', 'b', 'c'};
    rbuf_put(*b, data, 3);
    *result = 3;
    pthread_exit(result);
}

TEST(RingBufferThreadTest, PutThree) {
    rbuf b;
    rbuf_init(b, 1024);
    EXPECT_EQ(b.start, 0);
    EXPECT_EQ(rbuf_taken(b), 0);
    EXPECT_EQ(rbuf_available(b), rbuf_capacity(b));
    pthread_t thrd;
    if (0 == pthread_create(&thrd, NULL, put_three, &b)) {
        int* result;
        pthread_join(thrd, (void**)&result);
        EXPECT_EQ(*result, 3);
        EXPECT_EQ(rbuf_taken(b), 3);
    } else {
        FAIL();
    }
}

const timespec pump_sleep_duration{.tv_nsec = 100};
const timespec drain_sleep_duration{.tv_nsec = 1000};
const long int max_put_count = 10000;

void* pump_data(void *arg) {
    rbuf *b = (rbuf *)arg;
    const double start = wall_time();
    const double stop = start + 1.0;
    size_t &pumped = *((size_t *)malloc(sizeof(size_t)));
    pumped = 0;
    int count = 0;
    char buffer[500];
    while (1) {
        ++count;
        sprintf(buffer, "Let's write some data (count: %.5d)", count);
        size_t size = strlen(buffer);
        size_t offset = 0;
        int put_count = 0;
        while (size != offset && put_count < max_put_count) {
            ++put_count;
            offset +=
                rbuf_put(*b, (unsigned char *)buffer + offset, size - offset);
            nanosleep(&pump_sleep_duration, NULL);
        }
        pumped += offset;
        const double now = wall_time();
        if (now > stop)
            break;
    }
    pthread_exit(&pumped);
}

void* drain_data(void *arg) {
    rbuf *b = (rbuf *)arg;
    size_t &drained = *((size_t*)malloc(sizeof(size_t)));
    drained = 0;
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
        nanosleep(&drain_sleep_duration, NULL);
        const double now = wall_time();
        if (now > stop)
            break;
    }
    printf("zero reads: %d\n", zero_read);
    pthread_exit(&drained);
}

TEST(RingBufferThreadTest, PumpData) {
    rbuf b;
    rbuf_init(b, 1024);
    pthread_t pump_thrd;
    pthread_t drain_thrd;
    if (0 == pthread_create(&pump_thrd, NULL, pump_data, &b) &&
        0 == pthread_create(&drain_thrd, NULL, drain_data, &b)) {
        size_t* pump_result;
        size_t* drain_result;
        pthread_join(pump_thrd, (void**)&pump_result);
        pthread_join(drain_thrd, (void**)&drain_result);
        printf("pumped: %d\n", *((int*)pump_result));
        printf("drained: %d\n", *((int*)drain_result));
        EXPECT_EQ(*pump_result, *drain_result);
        free(pump_result);
        free(drain_result);
    } else {
        FAIL();
    }
    rbuf_destroy(b);
}