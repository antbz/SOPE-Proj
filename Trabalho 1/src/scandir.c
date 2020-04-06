#include "scandir.h"

void scan_dir(struct sduarg args) {
    DIR* dir;
    struct dirent* dirent;
    struct stat buf;

    if ((dir = opendir(args.path)) == NULL) {
        perror(args.path);
        logExit(3);
    }

    while ((dirent = readdir(dir)) != NULL) {
        if (strcmp(dirent->d_name, "..") == 0 || strcmp(dirent->d_name, ".") == 0) { continue; }

        char path[MAX_PATH];
        strcpy(path, args.path);
        strcat(path, dirent->d_name);

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
            logEntry(size, path);
            printf("%d\t%s\n", size, path);
        } else if (S_ISDIR(buf.st_mode)) {
            pid_t pid = fork();

            if (pid < 0) {
                perror("Fork error!");
                logExit(5);
            } else if (pid > 0) { // PARENT
                wait(NULL);
            } else { // CHILD
                strcpy(args.path, path);
                strcat(args.path, "/");
                
                scan_dir(args);
                logExit(0);
            }
        }
    }
}
