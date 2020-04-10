#include "sighandler.h"

extern pid_t child;

void sdu_sigint_handler(int sig) {
    logRecieveSig("SIGINT");

    if (child) {
        kill(-child, SIGSTOP);
        logSendSig("SIGSTOP", -child);
    }
    char opt;
    
    printf("\nDo you want to stop the program> [Y/N] ");
    fflush(stdout);

    while (1) {
        opt = getchar();

        if (opt == 'n' || opt == 'N') {
            kill(-child, SIGCONT);
            logSendSig("SIGCONT", -child);
            break;
        } else if (opt == 'Y' || opt == 'y') {
            kill(-child, SIGCONT);
            kill(-child, SIGTERM);
            logSendSig("SIGCONT", -child);
            logSendSig("SIGTERM", -child);
            logExit(10);
        }
    }
}

void sdu_sigcont_handler(int sig) {
    logRecieveSig("SIGCONT");
}

void sdu_sigterm_handler(int sig) {
    logRecieveSig("SIGTERM");
    logExit(10);
}
