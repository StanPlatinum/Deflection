#include <stdio.h>
#include <stdlib.h>
#include "CFICheck.h"
#include "enclave.h"

int mem_write(unsigned long* addr, unsigned long hex) {
    //memory write operation
    *addr = hex;
    return 0;
}

void enclave_main()
{
    puts("hello middle\n");
    unsigned long* buf = (unsigned long*)malloc(sizeof(unsigned long));
    buf = 0x7fffe67fb000;
    //WL: > HEAP BASE = 7fffe67bb000, < upper bound
    int rv = mem_write(buf, 0xDEADBEEF);
    puts("hello out\n");
	enclave_exit();
}

//WL: after disable ASLR
// program at 0x7fffe2fba3e8 (216196)
// .sgx.ssblob = 0x7fffe27ba3e8
// .sgx.calltg = 0x7fffe2bba3e8
// target string table at 0x7fffe2bba3e8 (1087)
// .sgxcode = 0x7fffe4fba3e8
// .sgxdata = 0x7fffe6fe13e8
// elf start = 0x7fffe2fba3e8
// elf end = 0x7fffe67ba3e8
// HEAP BASE = 7fffe67bb000
// upper bound: 0x7ffff0000000, lower bound: 0x7fffe0000000