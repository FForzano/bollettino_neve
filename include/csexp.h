/**
 * Utils for Canonical S-Expression applicative protocol
 **/

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <unistr.h>
#include <unistdio.h>
#include <unitypes.h>
#include <errno.h>
#include <limits.h>
#include "../include/extraio.h"

/**
 * createmex_c receive an array of string and the length (N_string) of array; return the address 
 * pointer of the uint8_t string with the message.
 * the message use a Canonical S-Expression protocol.
 * 
 * NB : this message is in C format string ('\0' terminated), send without it
 **/
uint8_t* createmex_c (char **string, int N_string);

/**
 * createmex_u8 receive an array of UTF-8 string and the length (N_string) of array; return the 
 * address pointer of the uint8_t string with the message.
 * the message use a Canonical S-Expression protocol.
 * 
 * NB : this message is in C format string ('\0' terminated), send without it
 **/
uint8_t* createmex_u8 (uint8_t **string, int N_string);

/**
 * getmexinfo put in output array the data of message in C format string ('\0' terminated).
 * N_output is the max number of element in output array.
 * 
 * Return -1 if output array buffer is too small.
 * Return -2 for conversion error.
 **/ 
int getmexinfo (uint8_t **output, int N_output, uint8_t *message);

/**
 * readmex read from fd one message and write in uint8_t *buff the address of the string
 * with message.
 * 
 * Return length of message.
 * Return an error code in case of fail.
 **/
int readmex (int fd, uint8_t **buff);

/**
 * return the error message.
 **/
char* rm_getmessage (int err);