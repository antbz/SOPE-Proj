#include "exreg.h"

clock_t start_time;
int log_fd;

int initReg() {
    start_time = clock();
    
    char* log_path = getenv("LOG_FILENAME");

    if (log_path == NULL) { log_path = DEFAULT_LOG; }
    
    log_fd = open(log_path, O_WRONLY | O_TRUNC | O_CREAT, 664);

    if (log_fd == -1) {
        perror("Failed to open/create log file");
        return 1;
    }

    return 0;
}

double elapsed_time() {
    clock_t current_time = clock();
    return ((double) (current_time - start_time)) / CLOCKS_PER_SEC;
}
