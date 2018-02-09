#ifndef TIMEUTILS_H
#define TIMEUTILS_H

#include <stdio.h>
#include <time.h>

struct timespec get_current_time();
double get_seconds_interval(struct timespec t_start, struct timespec t_end);

struct timespec get_current_time() {
   struct timespec t_curr={0,0};
   clock_gettime(CLOCK_MONOTONIC, &t_curr);
   return t_curr;
}

double get_seconds_interval(struct timespec t_start, struct timespec t_end) {
    return ((double)t_end.tv_sec + 1.0e-9*t_end.tv_nsec) - 
           ((double)t_start.tv_sec + 1.0e-9*t_start.tv_nsec);
}

#endif

