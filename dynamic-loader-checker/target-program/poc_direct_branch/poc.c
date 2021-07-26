#include <stdio.h>
#include <stdlib.h>


//#include "CFICheck.h"
//#include "CFICheck.c"

int mem_write(unsigned long* addr, unsigned long hex) {
	unsigned long cond = 0;
    goto exp;
ins:
    //memory write operation
    *addr = hex;
    cond = 1;

exp:
    if(cond == 0) {
        //direct jump
        goto ins;
    }
out:
    return 0;
}

int main()
{
    puts("hello in\n");
    // unsigned long magic = 0;
    puts("hello middle\n");
    unsigned long* buf = (unsigned long*)malloc(sizeof(unsigned long));
    // unsigned long * buf = 0x55555576e470;
    int rv = mem_write(buf, 0xDEADBEEF);
    puts("hello out\n");
	return 0;
}

// 0x55555576e470