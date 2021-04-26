#ifndef ENCLAVE_U_H__
#define ENCLAVE_U_H__

#include <stdint.h>
#include <wchar.h>
#include <stddef.h>
#include <string.h>
#include "sgx_edger8r.h" /* for sgx_status_t etc. */

#include "libelf_extra_types.h"

#include <stdlib.h> /* for size_t */

#define SGX_CAST(type, item) ((type)(item))

#ifdef __cplusplus
extern "C" {
#endif

#ifndef OCALL_PRINT_STRING_DEFINED__
#define OCALL_PRINT_STRING_DEFINED__
void SGX_UBRIDGE(SGX_NOCONVENTION, ocall_print_string, (const char* str));
#endif
#ifndef OCALL_SGX_PUSH_GADGET_DEFINED__
#define OCALL_SGX_PUSH_GADGET_DEFINED__
void SGX_UBRIDGE(SGX_NOCONVENTION, ocall_sgx_push_gadget, (unsigned long int gadget));
#endif
#ifndef OCALL_SGX_CLOCK_DEFINED__
#define OCALL_SGX_CLOCK_DEFINED__
clock_t SGX_UBRIDGE(SGX_NOCONVENTION, ocall_sgx_clock, (void));
#endif
#ifndef OCALL_SGX_TIME_DEFINED__
#define OCALL_SGX_TIME_DEFINED__
time_t SGX_UBRIDGE(SGX_NOCONVENTION, ocall_sgx_time, (time_t* t));
#endif
#ifndef OCALL_SGX_LOCALTIME_DEFINED__
#define OCALL_SGX_LOCALTIME_DEFINED__
struct tm* SGX_UBRIDGE(SGX_NOCONVENTION, ocall_sgx_localtime, (const time_t* timep));
#endif
#ifndef OCALL_SGX_GMTIME_DEFINED__
#define OCALL_SGX_GMTIME_DEFINED__
struct tm* SGX_UBRIDGE(SGX_NOCONVENTION, ocall_sgx_gmtime, (const time_t* timep));
#endif
#ifndef OCALL_SGX_MKTIME_DEFINED__
#define OCALL_SGX_MKTIME_DEFINED__
time_t SGX_UBRIDGE(SGX_NOCONVENTION, ocall_sgx_mktime, (struct tm* tmptr));
#endif
#ifndef OCALL_SGX_GETTIMEOFDAY_DEFINED__
#define OCALL_SGX_GETTIMEOFDAY_DEFINED__
int SGX_UBRIDGE(SGX_NOCONVENTION, ocall_sgx_gettimeofday, (struct timeval* tv, struct timezone* tz));
#endif
#ifndef OCALL_SGX_PUTS1TIME_DEFINED__
#define OCALL_SGX_PUTS1TIME_DEFINED__
int SGX_UBRIDGE(SGX_NOCONVENTION, ocall_sgx_puts1time, (char* str));
#endif
#ifndef OCALL_SGX_PUTS_DEFINED__
#define OCALL_SGX_PUTS_DEFINED__
int SGX_UBRIDGE(SGX_NOCONVENTION, ocall_sgx_puts, (const char* str));
#endif
#ifndef OCALL_SGX_OPEN_DEFINED__
#define OCALL_SGX_OPEN_DEFINED__
int SGX_UBRIDGE(SGX_NOCONVENTION, ocall_sgx_open, (const char* pathname, int flags, unsigned int mode));
#endif
#ifndef OCALL_SGX_CLOSE_DEFINED__
#define OCALL_SGX_CLOSE_DEFINED__
int SGX_UBRIDGE(SGX_NOCONVENTION, ocall_sgx_close, (int fd));
#endif
#ifndef OCALL_SGX_READ_DEFINED__
#define OCALL_SGX_READ_DEFINED__
ssize_t SGX_UBRIDGE(SGX_NOCONVENTION, ocall_sgx_read, (int fd, char* buf, size_t count));
#endif
#ifndef OCALL_SGX_WRITE_DEFINED__
#define OCALL_SGX_WRITE_DEFINED__
ssize_t SGX_UBRIDGE(SGX_NOCONVENTION, ocall_sgx_write, (int fd, const char* buf, size_t count));
#endif
#ifndef OCALL_SGX_LSEEK_DEFINED__
#define OCALL_SGX_LSEEK_DEFINED__
off_t SGX_UBRIDGE(SGX_NOCONVENTION, ocall_sgx_lseek, (int fildes, off_t offset, int whence));
#endif
#ifndef OCALL_SGX_SOCKET_DEFINED__
#define OCALL_SGX_SOCKET_DEFINED__
int SGX_UBRIDGE(SGX_NOCONVENTION, ocall_sgx_socket, (int af, int type, int protocol));
#endif
#ifndef OCALL_SGX_BIND_DEFINED__
#define OCALL_SGX_BIND_DEFINED__
int SGX_UBRIDGE(SGX_NOCONVENTION, ocall_sgx_bind, (int s, unsigned long int addr, size_t addr_size));
#endif
#ifndef OCALL_SGX_CONNECT_DEFINED__
#define OCALL_SGX_CONNECT_DEFINED__
int SGX_UBRIDGE(SGX_NOCONVENTION, ocall_sgx_connect, (int s, const struct sockaddr* addr, size_t addrlen));
#endif
#ifndef OCALL_SGX_LISTEN_DEFINED__
#define OCALL_SGX_LISTEN_DEFINED__
int SGX_UBRIDGE(SGX_NOCONVENTION, ocall_sgx_listen, (int s, int backlog));
#endif
#ifndef OCALL_SGX_ACCEPT_DEFINED__
#define OCALL_SGX_ACCEPT_DEFINED__
int SGX_UBRIDGE(SGX_NOCONVENTION, ocall_sgx_accept, (int s, struct sockaddr* addr, unsigned int addr_size, socklen_t* addrlen));
#endif
#ifndef OCALL_SGX_FSTAT_DEFINED__
#define OCALL_SGX_FSTAT_DEFINED__
int SGX_UBRIDGE(SGX_NOCONVENTION, ocall_sgx_fstat, (int fd, struct stat* buf));
#endif
#ifndef OCALL_SGX_SEND_DEFINED__
#define OCALL_SGX_SEND_DEFINED__
ssize_t SGX_UBRIDGE(SGX_NOCONVENTION, ocall_sgx_send, (int s, const void* buf, size_t len, int flags));
#endif
#ifndef OCALL_SGX_RECV_DEFINED__
#define OCALL_SGX_RECV_DEFINED__
ssize_t SGX_UBRIDGE(SGX_NOCONVENTION, ocall_sgx_recv, (int s, void* buf, size_t len, int flags));
#endif
#ifndef OCALL_SGX_SENDTO_DEFINED__
#define OCALL_SGX_SENDTO_DEFINED__
ssize_t SGX_UBRIDGE(SGX_NOCONVENTION, ocall_sgx_sendto, (int sockfd, const void* buf, size_t len, int flags, const struct sockaddr* dest_addr, size_t addrlen));
#endif
#ifndef OCALL_SGX_RECVFROM_DEFINED__
#define OCALL_SGX_RECVFROM_DEFINED__
ssize_t SGX_UBRIDGE(SGX_NOCONVENTION, ocall_sgx_recvfrom, (int s, void* buf, size_t len, int flags, struct sockaddr* dest_addr, unsigned int alen, socklen_t* addrlen));
#endif
#ifndef OCALL_SGX_GETHOSTNAME_DEFINED__
#define OCALL_SGX_GETHOSTNAME_DEFINED__
int SGX_UBRIDGE(SGX_NOCONVENTION, ocall_sgx_gethostname, (char* name, size_t namelen));
#endif
#ifndef OCALL_SGX_GETADDRINFO_DEFINED__
#define OCALL_SGX_GETADDRINFO_DEFINED__
int SGX_UBRIDGE(SGX_NOCONVENTION, ocall_sgx_getaddrinfo, (const char* node, const char* service, const struct addrinfo* hints, unsigned long int* res));
#endif
#ifndef OCALL_SGX_GETENV_DEFINED__
#define OCALL_SGX_GETENV_DEFINED__
char* SGX_UBRIDGE(SGX_NOCONVENTION, ocall_sgx_getenv, (const char* env));
#endif
#ifndef OCALL_SGX_GETSOCKNAME_DEFINED__
#define OCALL_SGX_GETSOCKNAME_DEFINED__
int SGX_UBRIDGE(SGX_NOCONVENTION, ocall_sgx_getsockname, (int s, struct sockaddr* name, unsigned int nlen, socklen_t* addrlen));
#endif
#ifndef OCALL_SGX_GETSOCKOPT_DEFINED__
#define OCALL_SGX_GETSOCKOPT_DEFINED__
int SGX_UBRIDGE(SGX_NOCONVENTION, ocall_sgx_getsockopt, (int s, int level, int optname, void* optval, unsigned int len, socklen_t* optlen));
#endif
#ifndef OCALL_SGX_GETSERVBYNAME_DEFINED__
#define OCALL_SGX_GETSERVBYNAME_DEFINED__
struct servent* SGX_UBRIDGE(SGX_NOCONVENTION, ocall_sgx_getservbyname, (const char* name, const char* proto));
#endif
#ifndef OCALL_SGX_GETPROTOBYNUMBER_DEFINED__
#define OCALL_SGX_GETPROTOBYNUMBER_DEFINED__
struct protoent* SGX_UBRIDGE(SGX_NOCONVENTION, ocall_sgx_getprotobynumber, (int proto));
#endif
#ifndef OCALL_SGX_SETSOCKOPT_DEFINED__
#define OCALL_SGX_SETSOCKOPT_DEFINED__
int SGX_UBRIDGE(SGX_NOCONVENTION, ocall_sgx_setsockopt, (int s, int level, int optname, const void* optval, size_t optlen));
#endif
#ifndef OCALL_SGX_HTONS_DEFINED__
#define OCALL_SGX_HTONS_DEFINED__
unsigned short int SGX_UBRIDGE(SGX_NOCONVENTION, ocall_sgx_htons, (unsigned short int hostshort));
#endif
#ifndef OCALL_SGX_HTONL_DEFINED__
#define OCALL_SGX_HTONL_DEFINED__
unsigned long int SGX_UBRIDGE(SGX_NOCONVENTION, ocall_sgx_htonl, (unsigned long int hostlong));
#endif
#ifndef OCALL_SGX_NTOHS_DEFINED__
#define OCALL_SGX_NTOHS_DEFINED__
unsigned short int SGX_UBRIDGE(SGX_NOCONVENTION, ocall_sgx_ntohs, (unsigned short int netshort));
#endif
#ifndef OCALL_SGX_NTOHL_DEFINED__
#define OCALL_SGX_NTOHL_DEFINED__
unsigned long int SGX_UBRIDGE(SGX_NOCONVENTION, ocall_sgx_ntohl, (unsigned long int netlong));
#endif
#ifndef OCALL_SGX_SIGNAL_DEFINED__
#define OCALL_SGX_SIGNAL_DEFINED__
sighandler_t SGX_UBRIDGE(SGX_NOCONVENTION, ocall_sgx_signal, (int signum, sighandler_t a));
#endif
#ifndef OCALL_SGX_SHUTDOWN_DEFINED__
#define OCALL_SGX_SHUTDOWN_DEFINED__
int SGX_UBRIDGE(SGX_NOCONVENTION, ocall_sgx_shutdown, (int a, int b));
#endif

sgx_status_t ecall_receive_data(sgx_enclave_id_t eid, char* buffer, int sz);
sgx_status_t ecall_receive_binary(sgx_enclave_id_t eid, char* buffer, int sz);
sgx_status_t ecall_receive_entrylabel(sgx_enclave_id_t eid, char* buffer, int sz);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
