#include <sys/socket.h>
#include "syscall.h"

#include <string.h>
#include <sgx-lib.h>

int bind(int fd, const struct sockaddr *addr, socklen_t len)
{
    sgx_stub_info *stub = (sgx_stub_info *)STUB_ADDR;

    stub->fcode = FUNC_BIND;
    stub->out_arg1 = fd;
    memcpy(stub->out_data1, addr, len);
    stub->out_arg2 = len;

    sgx_exit(stub->trampoline);

    return stub->in_arg1;
}
