#include <stdio.h>
#include <stdlib.h>


//#include "CFICheck.h"
//#include "CFICheck.c"

int mem_write(unsigned long* addr, unsigned long hex) {
    //memory write operation
    *addr = hex;
    return 0;
}

int main()
{
    puts("hello middle\n");
    unsigned long* buf = (unsigned long*)malloc(sizeof(unsigned long));
    // unsigned long * buf = 0x55555576e470;
    // int rv = mem_write(buf, 0xDEADBEEF);
    puts("hello out\n");
	return 0;
}
