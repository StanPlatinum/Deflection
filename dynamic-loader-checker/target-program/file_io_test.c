#include <stdio.h>
#include <fcntl.h>

#include "CFICheck.h"
#include "enclave.h"

#define PATH "./target-program/foo2.c"
#define BUFFER_SIZE 100

//Weijie:
// n should be larger than 1
// buf size should be less than n
int readline(int fd, char *buf, size_t n)
{
	for(int i = 0; i < n - 1; i++){
		char a;
		if (read(fd, &a, 1) != NULL) {
			if (a != '\n') {
				buf[i] = a;
			} else {
				buf[i] = '\n';
				buf[i+1] = '\0';
				return (i+1);
			}
		}
		else {
			buf[i+1] = '\0';
			return (i+1);
		}
	}
	buf[n] = '\0';
	return n;
}

void enclave_main(void)
{
	int in_fd;
	in_fd = open(PATH, O_RDONLY, S_IRUSR);

	if (in_fd == -1) {
		puts("PARAM file cannot be opened!\n")    ;
	}

	char buffer[BUFFER_SIZE];
	//if (read(in_fd, buffer, BUFFER_SIZE) != NULL) {
	if (readline(in_fd, buffer, BUFFER_SIZE) > 0) {
		puts(buffer);
	}

	enclave_exit();
}
