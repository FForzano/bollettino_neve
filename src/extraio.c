#include "../include/extraio.h"

/**
 * write_all write exactly n byte on fd from vptr buffer.
 **/
ssize_t write_all(int fd, const void *vptr, size_t n)
{
	size_t to_copy;
	ssize_t cc;
	const uint8_t *ptr;

	ptr = vptr;
	to_copy = n;
	while (to_copy > 0) {
		if ((cc = write(fd, ptr, to_copy)) <= 0) {
			if (cc < 0 && errno == EINTR)
				cc = 0;
			else
				return -1;
		}

		to_copy -= cc;
		ptr += cc;
	}
	return n;
}

/**
 * read_all read exactly n byte from fd if there are.
 * If there aren't n byte return number of read byte. 
 **/
ssize_t read_all(int fd, const void *vptr, size_t n){
	size_t counter=0;
	int Nread;
	uint8_t *ptr;

	ptr = (uint8_t *)vptr;
	while (counter != n){
		Nread=read(fd, ptr+counter, n-counter);

		if (Nread < 0) return (-1);
		if (Nread==0) {
			return (ssize_t) counter;
		}

		counter += (size_t) Nread;
	}
	return (ssize_t) counter;
}