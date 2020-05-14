#include "utils.h"

struct timespec start_time;

void printLog(int i, int pid, long tid, int dur, int pl, char* oper) {
    printf("%d ; %d ; %d ; %ld ; %d ; %d ; %s\n", (int)time(NULL), i, pid, tid, dur, pl, oper);
    fflush(stdout);
}

void setStart() {
    clock_gettime(CLOCK_MONOTONIC, &start_time);
}

double elapsedTime() {
    struct timespec current_time;
    clock_gettime(CLOCK_MONOTONIC, &current_time);

    return current_time.tv_sec - start_time.tv_sec + (current_time.tv_nsec - start_time.tv_nsec) * 1e-9;
}
