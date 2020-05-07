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

    double elapsed = (current_time.tv_sec - start_time.tv_sec) * 1e9;
    elapsed = (elapsed + (current_time.tv_nsec - start_time.tv_nsec)) * 1e-9;

    return elapsed;
}

int read_msg(int fifo_fd, char* msg, char delim) {
    char c;
    int i = 0;
    while (read(fifo_fd, &c, 1) > 0 && c != delim) {
        msg[i] = c;
        i++;
    }
    msg[i] = '\0'; // Terminate the string appropriately
    return strlen(msg);
}
