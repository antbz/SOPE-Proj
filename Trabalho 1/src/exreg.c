#include "exreg.h"

clock_t start_time;
int log_fd;

int initReg() {
    start_time = clock();
    
    char* log_path = getenv("LOG_FILENAME");

    if (log_path == NULL) { log_path = DEFAULT_LOG; }
    
    log_fd = open(log_path, O_WRONLY | O_TRUNC | O_CREAT, 0644);

    if (log_fd == -1) {
        perror("Failed to open/create log file");
        return 1;
    }

    return 0;
}

double elapsed_time() {
    clock_t current_time = clock();
    return ((double) (current_time - start_time)) / (CLOCKS_PER_SEC / (double) 1000);
}

void printLog(double instant, pid_t pid, char* action, char* info) {
    char line[MAX_LOG_LINE];
    sprintf(line, "%.2f - %.8d - %s - %s\n", instant, pid, action, info);

    write(log_fd, line, strlen(line));
}

void logExit(int status) {
    double instant = elapsed_time();
    char stat[3];
    sprintf(stat, "%d", status);

    printLog(instant, getpid(), "EXIT", stat);
    
    exit(status);
}

void logCreate(int argc, char* argv[]) {
    double instant = elapsed_time();
    char args[MAX_LOG_LINE] = "";
    
    for (int i = 0; i < argc; i++) {
        strcat(args, argv[i]);
        if (i != argc-1) { strcat(args, " "); }
    }

    printLog(instant, getpid(), "CREATE", args);
}
