#pragma once
#include <stdio.h>
#include <strings.h>
#include <time.h>

/**
 * \page time_util Supporting time routines
 */

timespec utc_now();
double timespec2double(const timespec &ts);
double wall_time();
