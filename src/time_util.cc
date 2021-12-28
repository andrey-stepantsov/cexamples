#include "time_util.hpp"

timespec utc_now() {
  timespec ts;
  timespec_get(&ts, TIME_UTC);
  return ts;
}
double timespec2double(const timespec &ts) {
  return ts.tv_sec + (ts.tv_nsec * 1e-9);
}

double wall_time() { return timespec2double(utc_now()); }
