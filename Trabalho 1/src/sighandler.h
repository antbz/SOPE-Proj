#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include "exreg.h"

/**
 * @brief SIGINT handler for parent process
*/
void sdu_sigint_handler(int sig);

/**
 * @brief SIGTSTP handler for child process
*/
void sdu_sigtstp_handler(int sig);

/**
 * @brief SIGCONT handler for child process
*/
void sdu_sigcont_handler(int sig);

/**
 * @brief SIGTERM handler for child process
*/
void sdu_sigterm_handler(int sig);
