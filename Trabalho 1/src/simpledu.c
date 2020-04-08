#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include "sduarg.h"
#include "exreg.h"
#include "scandir.h"
#include "sighandler.h"

extern pid_t main_prg;

int main(int argc, char* argv[]) {
    initReg();
    main_prg = getpgrp();
    
    logCreate(argc, argv);

    signal(SIGINT, sdu_sigint_handler);

    struct sduarg args = process_args(argc, argv);

    scan_dir(args);
    
    logExit(0);
}