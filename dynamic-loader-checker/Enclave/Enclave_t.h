#ifndef ENCLAVE_T_H__
#define ENCLAVE_T_H__

#include <stdint.h>
#include <wchar.h>
#include <stddef.h>
#include "sgx_edger8r.h" /* for sgx_ocall etc. */

#include "libelf_extra_types.h"

#include <stdlib.h> /* for size_t */

#define SGX_CAST(type, item) ((type)(item))

#ifdef __cplusplus
extern "C" {
#endif

void ecall_receive_data(char* buffer, int sz);
void ecall_receive_binary(char* buffer, int sz);
void ecall_receive_entrylabel(char* buffer, int sz);

sgx_status_t SGX_CDECL ocall_print_string(const char* str);
sgx_status_t SGX_CDECL ocall_sgx_push_gadget(unsigned long int gadget);
sgx_status_t SGX_CDECL ocall_sgx_clock(clock_t* retval);
sgx_status_t SGX_CDECL ocall_sgx_time(time_t* retval, time_t* t);
sgx_status_t SGX_CDECL ocall_sgx_localtime(struct tm** retval, const time_t* timep);
sgx_status_t SGX_CDECL ocall_sgx_gmtime(struct tm** retval, const time_t* timep);
sgx_status_t SGX_CDECL ocall_sgx_mktime(time_t* retval, struct tm* tmptr);
sgx_status_t SGX_CDECL ocall_sgx_gettimeofday(int* retval, struct timeval* tv, struct timezone* tz);
sgx_status_t SGX_CDECL ocall_sgx_puts1time(int* retval, char* str);
sgx_status_t SGX_CDECL ocall_sgx_puts(int* retval, const char* str);
sgx_status_t SGX_CDECL ocall_sgx_open(int* retval, const char* pathname, int flags, unsigned int mode);
sgx_status_t SGX_CDECL ocall_sgx_close(int* retval, int fd);
sgx_status_t SGX_CDECL ocall_sgx_read(ssize_t* retval, int fd, char* buf, size_t count);
sgx_status_t SGX_CDECL ocall_sgx_write(ssize_t* retval, int fd, const char* buf, size_t count);
sgx_status_t SGX_CDECL ocall_sgx_lseek(off_t* retval, int fildes, off_t offset, int whence);
sgx_status_t SGX_CDECL ocall_sgx_socket(int* retval, int af, int type, int protocol);
sgx_status_t SGX_CDECL ocall_sgx_bind(int* retval, int s, unsigned long int addr, size_t addr_size);
sgx_status_t SGX_CDECL ocall_sgx_connect(int* retval, int s, const struct sockaddr* addr, size_t addrlen);
sgx_status_t SGX_CDECL ocall_sgx_listen(int* retval, int s, int backlog);
sgx_status_t SGX_CDECL ocall_sgx_accept(int* retval, int s, struct sockaddr* addr, unsigned int addr_size, socklen_t* addrlen);
sgx_status_t SGX_CDECL ocall_sgx_fstat(int* retval, int fd, struct stat* buf);
sgx_status_t SGX_CDECL ocall_sgx_send(ssize_t* retval, int s, const void* buf, size_t len, int flags);
sgx_status_t SGX_CDECL ocall_sgx_recv(ssize_t* retval, int s, void* buf, size_t len, int flags);
sgx_status_t SGX_CDECL ocall_sgx_sendto(ssize_t* retval, int sockfd, const void* buf, size_t len, int flags, const struct sockaddr* dest_addr, size_t addrlen);
sgx_status_t SGX_CDECL ocall_sgx_recvfrom(ssize_t* retval, int s, void* buf, size_t len, int flags, struct sockaddr* dest_addr, unsigned int alen, socklen_t* addrlen);
sgx_status_t SGX_CDECL ocall_sgx_gethostname(int* retval, char* name, size_t namelen);
sgx_status_t SGX_CDECL ocall_sgx_getaddrinfo(int* retval, const char* node, const char* service, const struct addrinfo* hints, unsigned long int* res);
sgx_status_t SGX_CDECL ocall_sgx_getenv(char** retval, const char* env);
sgx_status_t SGX_CDECL ocall_sgx_getsockname(int* retval, int s, struct sockaddr* name, unsigned int nlen, socklen_t* addrlen);
sgx_status_t SGX_CDECL ocall_sgx_getsockopt(int* retval, int s, int level, int optname, void* optval, unsigned int len, socklen_t* optlen);
sgx_status_t SGX_CDECL ocall_sgx_getservbyname(struct servent** retval, const char* name, const char* proto);
sgx_status_t SGX_CDECL ocall_sgx_getprotobynumber(struct protoent** retval, int proto);
sgx_status_t SGX_CDECL ocall_sgx_setsockopt(int* retval, int s, int level, int optname, const void* optval, size_t optlen);
sgx_status_t SGX_CDECL ocall_sgx_htons(unsigned short int* retval, unsigned short int hostshort);
sgx_status_t SGX_CDECL ocall_sgx_htonl(unsigned long int* retval, unsigned long int hostlong);
sgx_status_t SGX_CDECL ocall_sgx_ntohs(unsigned short int* retval, unsigned short int netshort);
sgx_status_t SGX_CDECL ocall_sgx_ntohl(unsigned long int* retval, unsigned long int netlong);
sgx_status_t SGX_CDECL ocall_sgx_signal(sighandler_t* retval, int signum, sighandler_t a);
sgx_status_t SGX_CDECL ocall_sgx_shutdown(int* retval, int a, int b);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
