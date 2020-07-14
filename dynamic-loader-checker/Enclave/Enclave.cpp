#include <stdarg.h>
#include <stdio.h>      /* vsnprintf */

#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include "ocall_type.h"
typedef void (*sighandler_t)(int);

#include "Enclave.h"
#include "Enclave_t.h"  /* print_string */

#include "sgx_trts.h"   /* sgx_read_rand */

#include "elf.h"        /* ELF */

#ifdef LD_DEBUG
void printf(const char *fmt, ...)
{
    char buf[BUFSIZ] = {'\0'};
    va_list ap;
    va_start(ap, fmt);
    vsnprintf(buf, BUFSIZ, fmt, ap);
    va_end(ap);
    ocall_print_string(buf);
}
#define dlog(fmt, ...) printf(fmt "\n", __VA_ARGS__ )
#else
#define dlog(...)
#endif
#define pr_progress(s) dlog("\n=== bootstrap loader: %s ===", s)

extern char __elf_end;          /* defined in the linker script */
#define _HEAP_BASE (((addr_t)&__elf_end + 0xfff) & ~0xfff)

const unsigned long __sgx_data_ofs = 0x2027000;
//Xinyu & Weijie:
const unsigned long __sgx_code_ofs = 0x2000000;
#include "loader.cpp"
