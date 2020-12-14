/**
 * Server main procedure
 **/

#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <unitypes.h>
#include <unistr.h>
#include <unistdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <signal.h>
#include "../include/utils.h"
#include "../include/csexp.h"


void execute (int *pd_return, uint8_t *region_u8, uint8_t *N_u8);

char* medie_snow (char* buffer);