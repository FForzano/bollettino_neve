/**
 * extra high level function for I/O.
 **/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <limits.h>
#include <unitypes.h>

/**
 * write_all write exactly n byte on fd from vptr buffer.
 **/
ssize_t write_all(int fd, const void *vptr, size_t n);

/**
 * read_all read exactly n byte from fd if there are.
 * If there aren't n byte return number of read byte. 
 **/
ssize_t read_all(int fd, const void *vptr, size_t n);