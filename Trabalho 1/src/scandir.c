#include "scandir.h"

pid_t child = 0;
pid_t main_prg;

int scan_dir(struct sduarg args) {
    DIR* dir;
    struct dirent* dirent;
    struct stat buf;
    int cumulative = 0;

    if ((dir = opendir(args.path)) == NULL) {
        perror(args.path);
        logExit(3);
    }

    while ((dirent = readdir(dir)) != NULL) {
        char path[MAX_PATH];
        strcpy(path, args.path);
        strcat(path, dirent->d_name);

        if (strcmp(dirent->d_name, "..") == 0) { continue; }
        else if (strcmp(dirent->d_name, ".") == 0) {
            stat(path, &buf);
            if (args.bytes) {
                cumulative += buf.st_size;
            } else {
                cumulative += buf.st_blocks / 2;
            }
            continue;
        }

        if (args.deref) {
            if (stat(path, &buf)) {
                perror(dirent->d_name);
                logExit(4);
            }
        } else if (lstat(path, &buf)) {
            perror(dirent->d_name);
            logExit(4);
        }
        
        int size;

        if (S_ISREG(buf.st_mode) || S_ISLNK(buf.st_mode)) {
            if (args.bytes) {
                size = buf.st_size;
            } else {
                size = buf.st_blocks / 2;
            }
            cumulative += size;
            logEntry(size, path);
            if (args.all && args.max_depth > 0) { 
                char entry[MAX_LOG_LINE];
                if (args.bytes && !args.blocks) { sprintf(entry, "%d\t%s\n", size, path); }
                else if (args.bytes && args.blocks) { sprintf(entry, "%d\t%s\n", (int) ceil(size / (double) args.Bsize), path); }
                else { sprintf(entry, "%d\t%s\n", (int) ceil(size / ((double) args.Bsize / 1024.0)), path); }
                write(STDOUT_FILENO, entry, strlen(entry));
            }
        } else if (S_ISDIR(buf.st_mode)) {           
            int fd[2];

            if (pipe(fd) < 0) {
                perror("Pipe error!");
                logExit(6);
            }

            pid_t pid = fork();

            if (pid < 0) {
                perror("Fork error!");
                logExit(5);
            } else if (pid > 0) { // PARENT
                close(fd[WRITE]);
                
                if (getpgrp() == main_prg) { child = pid; }

                wait(NULL);
                
                if (!args.sepdir) {
                    int b;
                    read(fd[READ], &b, sizeof(int));
                    char message[MAX_LOG_LINE];
                    sprintf(message, "%d", b);
                    logRecievePipe(message);
                    cumulative += b;
                }
            } else { // CHILD
                if (getpgrp() == main_prg) {
                    setpgid(pid, getpid());
                }
                signal(SIGCONT, sdu_sigcont_handler);
                signal(SIGTERM, sdu_sigterm_handler);
                
                close(fd[READ]);
                
                strcpy(args.path, path);
                strcat(args.path, "/");
                
                if (args.max_depth != __INT_MAX__) { args.max_depth--; }

                logCreateFork(&args);

                int s = scan_dir(args);
                if (!args.sepdir) {
                    char message[MAX_LOG_LINE];
                    sprintf(message, "%d", s);
                    logSendPipe(message);
                    write(fd[WRITE], &s, sizeof(s));
                }
                
                logExit(0);
            }
        }
    }
    
    child = 0;

    args.path[strlen(args.path)-1] = 0;
    logEntry(cumulative, args.path);
    if(args.max_depth >= 0){ 
        char entry[MAX_LOG_LINE];
        if (args.bytes && !args.blocks) { sprintf(entry, "%d\t%s\n", cumulative, args.path); }
        else if (args.bytes && args.blocks) { sprintf(entry, "%d\t%s\n", (int) ceil(cumulative / (double) args.Bsize), args.path); }
        else { sprintf(entry, "%d\t%s\n", (int) ceil(cumulative / ((double) args.Bsize / 1024.0)), args.path); }
        write(STDOUT_FILENO, entry, strlen(entry));
    }
    
    return cumulative;
}
