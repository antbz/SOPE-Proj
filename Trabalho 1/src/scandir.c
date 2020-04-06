#include "scandir.h"

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
                cumulative += buf.st_blocks * 512 / args.Bsize;
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
                size = buf.st_blocks * 512 / args.Bsize;
            }
            cumulative += size;
            logEntry(size, path);
            if (args.all) { printf("%d\t%s\n", size, path); }
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
                
                wait(NULL);
                
                int b;
                read(fd[READ], &b, sizeof(int));
                
                cumulative += b;
            } else { // CHILD
                close(fd[READ]);
                
                strcpy(args.path, path);
                strcat(args.path, "/");
                
                logCreateFork(&args);
                
                int s = scan_dir(args);
                write(fd[WRITE], &s, sizeof(s));
                
                logExit(0);
            }
        }
    }

    printf("%d\t%s\n", cumulative, args.path);
    return cumulative;
}
