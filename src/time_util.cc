#include "time_util.hpp"

timespec utc_now() {
    timespec ts;
    timespec_get(&ts, TIME_UTC);
    return ts;
}

/**
 * @brief converts @ref timespec to @ref double (seconds since epoch)
 * 
 * @param ts time to convert 
 * @return double 
 */
double timespec2double(const timespec &ts) {
    return ts.tv_sec + (ts.tv_nsec * 1e-9);
}

/**
 * @brief returns current UTC time as double (seconds since epoch)
 * 
 * @return double 
 */
double wall_time() { return timespec2double(utc_now()); }
