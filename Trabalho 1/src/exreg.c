#include "exreg.h"

clock_t start_time;
int log_fd;

void initReg() {
    start_time = clock(); // Store start time
    
    char* log_path = getenv("LOG_FILENAME");

    if (log_path == NULL) { log_path = DEFAULT_LOG; } // If no env. var was defined, use default value
    
    log_fd = open(log_path, O_WRONLY | O_TRUNC | O_CREAT, 0644); // Open file for writing, discard previous content and create with rw-r--r-- permissions

    if (log_fd == -1) {
        perror("Failed to open/create log file");
        exit(2);
    }
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

    close(log_fd);
    
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

void logCreateFork(struct sduarg *args){
    double instant = elapsed_time();
    char output[MAX_LOG_LINE] = "";
    strcpy(output,"-l ");
    strcat(output,args->path);
    if(args->all == 1){strcat(output," -a");}
    if(args->bytes == 1){strcat(output," -b");}
    else if(args->Bsize!=1024){
        char number[12];
        sprintf(number, " -B %d", args->Bsize);
        strcat(output,number);
    }
    if(args->deref == 1){strcat(output," -L");}
    if(args->sepdir == 1){strcat(output," -S");}

    if(args->max_depth!=-1){
        char number[28];
        sprintf(number, " --max_depth=%d", args->max_depth);
        strcat(output,number);
    }

    printLog(instant, getpid(), "CREATE", output);
}

void logEntry(int size, char* path) {
    double instant = elapsed_time();
    char entry[MAX_LOG_LINE] = "";
    
    sprintf(entry, "%d %s", size, path);

    printLog(instant, getpid(), "ENTRY", entry);
}
