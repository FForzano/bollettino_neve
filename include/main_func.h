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

/**
 * execute is the main function of the server main.
 * It exec a "sort -r -k3 -n /home/federico/bollettino_neve/<region>.txt | head -nN"
 * and direct the output in the return pipe.
 **/
void execute (int *pd_return, uint8_t *region_u8, uint8_t *N_u8);

/**
 * medie_snow calcolate the medie of centimeters of snow in the requested ski resort.
 * Return a pointer of string with the medie, return NULL if there are any error.
 **/
char* medie_snow (char* buffer);