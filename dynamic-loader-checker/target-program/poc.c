#include "enclave.h"

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

void enclave_main()
{
    unsigned long* buf = (unsigned long*)malloc(sizeof(unsigned long));
    int rv = mem_write(buf, 0xDEADBEEF);
    puts("hello out\n");
	enclave_exit();
}