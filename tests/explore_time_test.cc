#include "time_util.hpp"
#include <gtest/gtest.h>
#include <stdio.h>
#include <strings.h>
// #include <threads.h>

TEST(ExploreTimeTest, Nanosecond) {
  EXPECT_EQ(1.0 / 10.0, 1e-1);
  EXPECT_EQ(1.0 / 1000000000.0, 1e-9);
}

TEST(ExploreTimeTest, ToDouble) {
  const timespec now = utc_now();
  const double now_double = timespec2double(now);
  EXPECT_NEAR(now_double, now.tv_sec, 1);
}

void* do_nothing(void *arg) {
  const timespec sleep_duration{.tv_nsec = 1000};
  const double start = wall_time();
  const double report_step = 1.0;
  const double stop = start + 5.0;
  double report_after = start + report_step;
  while (1) {
    nanosleep(&sleep_duration, NULL);
    const double now = wall_time();
    if (report_after < now) {
      report_after = now + report_step;
      printf("%.3f\n", now);
    }
    if (now > stop)
      break;
  }
  int* result = (int*)malloc(sizeof(int));
  *result = 101;
  pthread_exit(result);
}

TEST(ExploreTimeTest, DoNothing) {
  pthread_t nothing_thrd;
  if (0 == pthread_create(&nothing_thrd, NULL, do_nothing, NULL)) {
    int* nothing_result;
    pthread_join(nothing_thrd, (void**)&nothing_result);
    printf("nothing: %d\n", *nothing_result);
    EXPECT_EQ(*nothing_result, 101);
    SUCCEED();
  } else {
    FAIL();
  }
}
