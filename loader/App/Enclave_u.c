#include "Enclave_u.h"
#include <errno.h>

typedef struct ms_ecall_receive_binary_t {
	char* ms_buffer;
	int ms_sz;
} ms_ecall_receive_binary_t;

typedef struct ms_ecall_receive_data_t {
	char* ms_buffer;
	int ms_sz;
} ms_ecall_receive_data_t;

typedef struct ms_ocall_print_string_t {
	const char* ms_str;
} ms_ocall_print_string_t;

typedef struct ms_ocall_sgx_push_gadget_t {
	unsigned long int ms_gadget;
} ms_ocall_sgx_push_gadget_t;

typedef struct ms_ocall_sgx_clock_t {
	clock_t ms_retval;
} ms_ocall_sgx_clock_t;

typedef struct ms_ocall_sgx_time_t {
	time_t ms_retval;
	time_t* ms_t;
} ms_ocall_sgx_time_t;

typedef struct ms_ocall_sgx_localtime_t {
	struct tm* ms_retval;
	const time_t* ms_timep;
} ms_ocall_sgx_localtime_t;

typedef struct ms_ocall_sgx_gmtime_t {
	struct tm* ms_retval;
	const time_t* ms_timep;
} ms_ocall_sgx_gmtime_t;

typedef struct ms_ocall_sgx_mktime_t {
	time_t ms_retval;
	struct tm* ms_tmptr;
} ms_ocall_sgx_mktime_t;

typedef struct ms_ocall_sgx_gettimeofday_t {
	int ms_retval;
	struct timeval* ms_tv;
	struct timezone* ms_tz;
} ms_ocall_sgx_gettimeofday_t;

typedef struct ms_ocall_sgx_puts_t {
	int ms_retval;
	const char* ms_str;
} ms_ocall_sgx_puts_t;

typedef struct ms_ocall_sgx_open_t {
	int ms_retval;
	const char* ms_pathname;
	int ms_flags;
	unsigned int ms_mode;
} ms_ocall_sgx_open_t;

typedef struct ms_ocall_sgx_close_t {
	int ms_retval;
	int ms_fd;
} ms_ocall_sgx_close_t;

typedef struct ms_ocall_sgx_read_t {
	ssize_t ms_retval;
	int ms_fd;
	char* ms_buf;
	size_t ms_count;
} ms_ocall_sgx_read_t;

typedef struct ms_ocall_sgx_write_t {
	ssize_t ms_retval;
	int ms_fd;
	const char* ms_buf;
	size_t ms_count;
} ms_ocall_sgx_write_t;

typedef struct ms_ocall_sgx_lseek_t {
	off_t ms_retval;
	int ms_fildes;
	off_t ms_offset;
	int ms_whence;
} ms_ocall_sgx_lseek_t;

typedef struct ms_ocall_sgx_socket_t {
	int ms_retval;
	int ms_af;
	int ms_type;
	int ms_protocol;
} ms_ocall_sgx_socket_t;

typedef struct ms_ocall_sgx_bind_t {
	int ms_retval;
	int ms_s;
	unsigned long int ms_addr;
	size_t ms_addr_size;
} ms_ocall_sgx_bind_t;

typedef struct ms_ocall_sgx_connect_t {
	int ms_retval;
	int ms_s;
	const struct sockaddr* ms_addr;
	size_t ms_addrlen;
} ms_ocall_sgx_connect_t;

typedef struct ms_ocall_sgx_listen_t {
	int ms_retval;
	int ms_s;
	int ms_backlog;
} ms_ocall_sgx_listen_t;

typedef struct ms_ocall_sgx_accept_t {
	int ms_retval;
	int ms_s;
	struct sockaddr* ms_addr;
	unsigned int ms_addr_size;
	socklen_t* ms_addrlen;
} ms_ocall_sgx_accept_t;

typedef struct ms_ocall_sgx_fstat_t {
	int ms_retval;
	int ms_fd;
	struct stat* ms_buf;
} ms_ocall_sgx_fstat_t;

typedef struct ms_ocall_sgx_send_t {
	ssize_t ms_retval;
	int ms_s;
	const void* ms_buf;
	size_t ms_len;
	int ms_flags;
} ms_ocall_sgx_send_t;

typedef struct ms_ocall_sgx_recv_t {
	ssize_t ms_retval;
	int ms_s;
	void* ms_buf;
	size_t ms_len;
	int ms_flags;
} ms_ocall_sgx_recv_t;

typedef struct ms_ocall_sgx_sendto_t {
	ssize_t ms_retval;
	int ms_sockfd;
	const void* ms_buf;
	size_t ms_len;
	int ms_flags;
	const struct sockaddr* ms_dest_addr;
	size_t ms_addrlen;
} ms_ocall_sgx_sendto_t;

typedef struct ms_ocall_sgx_recvfrom_t {
	ssize_t ms_retval;
	int ms_s;
	void* ms_buf;
	size_t ms_len;
	int ms_flags;
	struct sockaddr* ms_dest_addr;
	unsigned int ms_alen;
	socklen_t* ms_addrlen;
} ms_ocall_sgx_recvfrom_t;

typedef struct ms_ocall_sgx_gethostname_t {
	int ms_retval;
	char* ms_name;
	size_t ms_namelen;
} ms_ocall_sgx_gethostname_t;

typedef struct ms_ocall_sgx_getaddrinfo_t {
	int ms_retval;
	const char* ms_node;
	const char* ms_service;
	const struct addrinfo* ms_hints;
	unsigned long int* ms_res;
} ms_ocall_sgx_getaddrinfo_t;

typedef struct ms_ocall_sgx_getenv_t {
	char* ms_retval;
	const char* ms_env;
} ms_ocall_sgx_getenv_t;

typedef struct ms_ocall_sgx_getsockname_t {
	int ms_retval;
	int ms_s;
	struct sockaddr* ms_name;
	unsigned int ms_nlen;
	socklen_t* ms_addrlen;
} ms_ocall_sgx_getsockname_t;

typedef struct ms_ocall_sgx_getsockopt_t {
	int ms_retval;
	int ms_s;
	int ms_level;
	int ms_optname;
	void* ms_optval;
	unsigned int ms_len;
	socklen_t* ms_optlen;
} ms_ocall_sgx_getsockopt_t;

typedef struct ms_ocall_sgx_getservbyname_t {
	struct servent* ms_retval;
	const char* ms_name;
	const char* ms_proto;
} ms_ocall_sgx_getservbyname_t;

typedef struct ms_ocall_sgx_getprotobynumber_t {
	struct protoent* ms_retval;
	int ms_proto;
} ms_ocall_sgx_getprotobynumber_t;

typedef struct ms_ocall_sgx_setsockopt_t {
	int ms_retval;
	int ms_s;
	int ms_level;
	int ms_optname;
	const void* ms_optval;
	size_t ms_optlen;
} ms_ocall_sgx_setsockopt_t;

typedef struct ms_ocall_sgx_htons_t {
	unsigned short int ms_retval;
	unsigned short int ms_hostshort;
} ms_ocall_sgx_htons_t;

typedef struct ms_ocall_sgx_htonl_t {
	unsigned long int ms_retval;
	unsigned long int ms_hostlong;
} ms_ocall_sgx_htonl_t;

typedef struct ms_ocall_sgx_ntohs_t {
	unsigned short int ms_retval;
	unsigned short int ms_netshort;
} ms_ocall_sgx_ntohs_t;

typedef struct ms_ocall_sgx_ntohl_t {
	unsigned long int ms_retval;
	unsigned long int ms_netlong;
} ms_ocall_sgx_ntohl_t;

typedef struct ms_ocall_sgx_signal_t {
	sighandler_t ms_retval;
	int ms_signum;
	sighandler_t ms_a;
} ms_ocall_sgx_signal_t;

typedef struct ms_ocall_sgx_shutdown_t {
	int ms_retval;
	int ms_a;
	int ms_b;
} ms_ocall_sgx_shutdown_t;

static sgx_status_t SGX_CDECL Enclave_ocall_print_string(void* pms)
{
	ms_ocall_print_string_t* ms = SGX_CAST(ms_ocall_print_string_t*, pms);
	ocall_print_string(ms->ms_str);

	return SGX_SUCCESS;
}

static sgx_status_t SGX_CDECL Enclave_ocall_sgx_push_gadget(void* pms)
{
	ms_ocall_sgx_push_gadget_t* ms = SGX_CAST(ms_ocall_sgx_push_gadget_t*, pms);
	ocall_sgx_push_gadget(ms->ms_gadget);

	return SGX_SUCCESS;
}

static sgx_status_t SGX_CDECL Enclave_ocall_sgx_clock(void* pms)
{
	ms_ocall_sgx_clock_t* ms = SGX_CAST(ms_ocall_sgx_clock_t*, pms);
	ms->ms_retval = ocall_sgx_clock();

	return SGX_SUCCESS;
}

static sgx_status_t SGX_CDECL Enclave_ocall_sgx_time(void* pms)
{
	ms_ocall_sgx_time_t* ms = SGX_CAST(ms_ocall_sgx_time_t*, pms);
	ms->ms_retval = ocall_sgx_time(ms->ms_t);

	return SGX_SUCCESS;
}

static sgx_status_t SGX_CDECL Enclave_ocall_sgx_localtime(void* pms)
{
	ms_ocall_sgx_localtime_t* ms = SGX_CAST(ms_ocall_sgx_localtime_t*, pms);
	ms->ms_retval = ocall_sgx_localtime(ms->ms_timep);

	return SGX_SUCCESS;
}

static sgx_status_t SGX_CDECL Enclave_ocall_sgx_gmtime(void* pms)
{
	ms_ocall_sgx_gmtime_t* ms = SGX_CAST(ms_ocall_sgx_gmtime_t*, pms);
	ms->ms_retval = ocall_sgx_gmtime(ms->ms_timep);

	return SGX_SUCCESS;
}

static sgx_status_t SGX_CDECL Enclave_ocall_sgx_mktime(void* pms)
{
	ms_ocall_sgx_mktime_t* ms = SGX_CAST(ms_ocall_sgx_mktime_t*, pms);
	ms->ms_retval = ocall_sgx_mktime(ms->ms_tmptr);

	return SGX_SUCCESS;
}

static sgx_status_t SGX_CDECL Enclave_ocall_sgx_gettimeofday(void* pms)
{
	ms_ocall_sgx_gettimeofday_t* ms = SGX_CAST(ms_ocall_sgx_gettimeofday_t*, pms);
	ms->ms_retval = ocall_sgx_gettimeofday(ms->ms_tv, ms->ms_tz);

	return SGX_SUCCESS;
}

static sgx_status_t SGX_CDECL Enclave_ocall_sgx_puts(void* pms)
{
	ms_ocall_sgx_puts_t* ms = SGX_CAST(ms_ocall_sgx_puts_t*, pms);
	ms->ms_retval = ocall_sgx_puts(ms->ms_str);

	return SGX_SUCCESS;
}

static sgx_status_t SGX_CDECL Enclave_ocall_sgx_open(void* pms)
{
	ms_ocall_sgx_open_t* ms = SGX_CAST(ms_ocall_sgx_open_t*, pms);
	ms->ms_retval = ocall_sgx_open(ms->ms_pathname, ms->ms_flags, ms->ms_mode);

	return SGX_SUCCESS;
}

static sgx_status_t SGX_CDECL Enclave_ocall_sgx_close(void* pms)
{
	ms_ocall_sgx_close_t* ms = SGX_CAST(ms_ocall_sgx_close_t*, pms);
	ms->ms_retval = ocall_sgx_close(ms->ms_fd);

	return SGX_SUCCESS;
}

static sgx_status_t SGX_CDECL Enclave_ocall_sgx_read(void* pms)
{
	ms_ocall_sgx_read_t* ms = SGX_CAST(ms_ocall_sgx_read_t*, pms);
	ms->ms_retval = ocall_sgx_read(ms->ms_fd, ms->ms_buf, ms->ms_count);

	return SGX_SUCCESS;
}

static sgx_status_t SGX_CDECL Enclave_ocall_sgx_write(void* pms)
{
	ms_ocall_sgx_write_t* ms = SGX_CAST(ms_ocall_sgx_write_t*, pms);
	ms->ms_retval = ocall_sgx_write(ms->ms_fd, ms->ms_buf, ms->ms_count);

	return SGX_SUCCESS;
}

static sgx_status_t SGX_CDECL Enclave_ocall_sgx_lseek(void* pms)
{
	ms_ocall_sgx_lseek_t* ms = SGX_CAST(ms_ocall_sgx_lseek_t*, pms);
	ms->ms_retval = ocall_sgx_lseek(ms->ms_fildes, ms->ms_offset, ms->ms_whence);

	return SGX_SUCCESS;
}

static sgx_status_t SGX_CDECL Enclave_ocall_sgx_socket(void* pms)
{
	ms_ocall_sgx_socket_t* ms = SGX_CAST(ms_ocall_sgx_socket_t*, pms);
	ms->ms_retval = ocall_sgx_socket(ms->ms_af, ms->ms_type, ms->ms_protocol);

	return SGX_SUCCESS;
}

static sgx_status_t SGX_CDECL Enclave_ocall_sgx_bind(void* pms)
{
	ms_ocall_sgx_bind_t* ms = SGX_CAST(ms_ocall_sgx_bind_t*, pms);
	ms->ms_retval = ocall_sgx_bind(ms->ms_s, ms->ms_addr, ms->ms_addr_size);

	return SGX_SUCCESS;
}

static sgx_status_t SGX_CDECL Enclave_ocall_sgx_connect(void* pms)
{
	ms_ocall_sgx_connect_t* ms = SGX_CAST(ms_ocall_sgx_connect_t*, pms);
	ms->ms_retval = ocall_sgx_connect(ms->ms_s, ms->ms_addr, ms->ms_addrlen);

	return SGX_SUCCESS;
}

static sgx_status_t SGX_CDECL Enclave_ocall_sgx_listen(void* pms)
{
	ms_ocall_sgx_listen_t* ms = SGX_CAST(ms_ocall_sgx_listen_t*, pms);
	ms->ms_retval = ocall_sgx_listen(ms->ms_s, ms->ms_backlog);

	return SGX_SUCCESS;
}

static sgx_status_t SGX_CDECL Enclave_ocall_sgx_accept(void* pms)
{
	ms_ocall_sgx_accept_t* ms = SGX_CAST(ms_ocall_sgx_accept_t*, pms);
	ms->ms_retval = ocall_sgx_accept(ms->ms_s, ms->ms_addr, ms->ms_addr_size, ms->ms_addrlen);

	return SGX_SUCCESS;
}

static sgx_status_t SGX_CDECL Enclave_ocall_sgx_fstat(void* pms)
{
	ms_ocall_sgx_fstat_t* ms = SGX_CAST(ms_ocall_sgx_fstat_t*, pms);
	ms->ms_retval = ocall_sgx_fstat(ms->ms_fd, ms->ms_buf);

	return SGX_SUCCESS;
}

static sgx_status_t SGX_CDECL Enclave_ocall_sgx_send(void* pms)
{
	ms_ocall_sgx_send_t* ms = SGX_CAST(ms_ocall_sgx_send_t*, pms);
	ms->ms_retval = ocall_sgx_send(ms->ms_s, ms->ms_buf, ms->ms_len, ms->ms_flags);

	return SGX_SUCCESS;
}

static sgx_status_t SGX_CDECL Enclave_ocall_sgx_recv(void* pms)
{
	ms_ocall_sgx_recv_t* ms = SGX_CAST(ms_ocall_sgx_recv_t*, pms);
	ms->ms_retval = ocall_sgx_recv(ms->ms_s, ms->ms_buf, ms->ms_len, ms->ms_flags);

	return SGX_SUCCESS;
}

static sgx_status_t SGX_CDECL Enclave_ocall_sgx_sendto(void* pms)
{
	ms_ocall_sgx_sendto_t* ms = SGX_CAST(ms_ocall_sgx_sendto_t*, pms);
	ms->ms_retval = ocall_sgx_sendto(ms->ms_sockfd, ms->ms_buf, ms->ms_len, ms->ms_flags, ms->ms_dest_addr, ms->ms_addrlen);

	return SGX_SUCCESS;
}

static sgx_status_t SGX_CDECL Enclave_ocall_sgx_recvfrom(void* pms)
{
	ms_ocall_sgx_recvfrom_t* ms = SGX_CAST(ms_ocall_sgx_recvfrom_t*, pms);
	ms->ms_retval = ocall_sgx_recvfrom(ms->ms_s, ms->ms_buf, ms->ms_len, ms->ms_flags, ms->ms_dest_addr, ms->ms_alen, ms->ms_addrlen);

	return SGX_SUCCESS;
}

static sgx_status_t SGX_CDECL Enclave_ocall_sgx_gethostname(void* pms)
{
	ms_ocall_sgx_gethostname_t* ms = SGX_CAST(ms_ocall_sgx_gethostname_t*, pms);
	ms->ms_retval = ocall_sgx_gethostname(ms->ms_name, ms->ms_namelen);

	return SGX_SUCCESS;
}

static sgx_status_t SGX_CDECL Enclave_ocall_sgx_getaddrinfo(void* pms)
{
	ms_ocall_sgx_getaddrinfo_t* ms = SGX_CAST(ms_ocall_sgx_getaddrinfo_t*, pms);
	ms->ms_retval = ocall_sgx_getaddrinfo(ms->ms_node, ms->ms_service, ms->ms_hints, ms->ms_res);

	return SGX_SUCCESS;
}

static sgx_status_t SGX_CDECL Enclave_ocall_sgx_getenv(void* pms)
{
	ms_ocall_sgx_getenv_t* ms = SGX_CAST(ms_ocall_sgx_getenv_t*, pms);
	ms->ms_retval = ocall_sgx_getenv(ms->ms_env);

	return SGX_SUCCESS;
}

static sgx_status_t SGX_CDECL Enclave_ocall_sgx_getsockname(void* pms)
{
	ms_ocall_sgx_getsockname_t* ms = SGX_CAST(ms_ocall_sgx_getsockname_t*, pms);
	ms->ms_retval = ocall_sgx_getsockname(ms->ms_s, ms->ms_name, ms->ms_nlen, ms->ms_addrlen);

	return SGX_SUCCESS;
}

static sgx_status_t SGX_CDECL Enclave_ocall_sgx_getsockopt(void* pms)
{
	ms_ocall_sgx_getsockopt_t* ms = SGX_CAST(ms_ocall_sgx_getsockopt_t*, pms);
	ms->ms_retval = ocall_sgx_getsockopt(ms->ms_s, ms->ms_level, ms->ms_optname, ms->ms_optval, ms->ms_len, ms->ms_optlen);

	return SGX_SUCCESS;
}

static sgx_status_t SGX_CDECL Enclave_ocall_sgx_getservbyname(void* pms)
{
	ms_ocall_sgx_getservbyname_t* ms = SGX_CAST(ms_ocall_sgx_getservbyname_t*, pms);
	ms->ms_retval = ocall_sgx_getservbyname(ms->ms_name, ms->ms_proto);

	return SGX_SUCCESS;
}

static sgx_status_t SGX_CDECL Enclave_ocall_sgx_getprotobynumber(void* pms)
{
	ms_ocall_sgx_getprotobynumber_t* ms = SGX_CAST(ms_ocall_sgx_getprotobynumber_t*, pms);
	ms->ms_retval = ocall_sgx_getprotobynumber(ms->ms_proto);

	return SGX_SUCCESS;
}

static sgx_status_t SGX_CDECL Enclave_ocall_sgx_setsockopt(void* pms)
{
	ms_ocall_sgx_setsockopt_t* ms = SGX_CAST(ms_ocall_sgx_setsockopt_t*, pms);
	ms->ms_retval = ocall_sgx_setsockopt(ms->ms_s, ms->ms_level, ms->ms_optname, ms->ms_optval, ms->ms_optlen);

	return SGX_SUCCESS;
}

static sgx_status_t SGX_CDECL Enclave_ocall_sgx_htons(void* pms)
{
	ms_ocall_sgx_htons_t* ms = SGX_CAST(ms_ocall_sgx_htons_t*, pms);
	ms->ms_retval = ocall_sgx_htons(ms->ms_hostshort);

	return SGX_SUCCESS;
}

static sgx_status_t SGX_CDECL Enclave_ocall_sgx_htonl(void* pms)
{
	ms_ocall_sgx_htonl_t* ms = SGX_CAST(ms_ocall_sgx_htonl_t*, pms);
	ms->ms_retval = ocall_sgx_htonl(ms->ms_hostlong);

	return SGX_SUCCESS;
}

static sgx_status_t SGX_CDECL Enclave_ocall_sgx_ntohs(void* pms)
{
	ms_ocall_sgx_ntohs_t* ms = SGX_CAST(ms_ocall_sgx_ntohs_t*, pms);
	ms->ms_retval = ocall_sgx_ntohs(ms->ms_netshort);

	return SGX_SUCCESS;
}

static sgx_status_t SGX_CDECL Enclave_ocall_sgx_ntohl(void* pms)
{
	ms_ocall_sgx_ntohl_t* ms = SGX_CAST(ms_ocall_sgx_ntohl_t*, pms);
	ms->ms_retval = ocall_sgx_ntohl(ms->ms_netlong);

	return SGX_SUCCESS;
}

static sgx_status_t SGX_CDECL Enclave_ocall_sgx_signal(void* pms)
{
	ms_ocall_sgx_signal_t* ms = SGX_CAST(ms_ocall_sgx_signal_t*, pms);
	ms->ms_retval = ocall_sgx_signal(ms->ms_signum, ms->ms_a);

	return SGX_SUCCESS;
}

static sgx_status_t SGX_CDECL Enclave_ocall_sgx_shutdown(void* pms)
{
	ms_ocall_sgx_shutdown_t* ms = SGX_CAST(ms_ocall_sgx_shutdown_t*, pms);
	ms->ms_retval = ocall_sgx_shutdown(ms->ms_a, ms->ms_b);

	return SGX_SUCCESS;
}

static const struct {
	size_t nr_ocall;
	void * table[38];
} ocall_table_Enclave = {
	38,
	{
		(void*)Enclave_ocall_print_string,
		(void*)Enclave_ocall_sgx_push_gadget,
		(void*)Enclave_ocall_sgx_clock,
		(void*)Enclave_ocall_sgx_time,
		(void*)Enclave_ocall_sgx_localtime,
		(void*)Enclave_ocall_sgx_gmtime,
		(void*)Enclave_ocall_sgx_mktime,
		(void*)Enclave_ocall_sgx_gettimeofday,
		(void*)Enclave_ocall_sgx_puts,
		(void*)Enclave_ocall_sgx_open,
		(void*)Enclave_ocall_sgx_close,
		(void*)Enclave_ocall_sgx_read,
		(void*)Enclave_ocall_sgx_write,
		(void*)Enclave_ocall_sgx_lseek,
		(void*)Enclave_ocall_sgx_socket,
		(void*)Enclave_ocall_sgx_bind,
		(void*)Enclave_ocall_sgx_connect,
		(void*)Enclave_ocall_sgx_listen,
		(void*)Enclave_ocall_sgx_accept,
		(void*)Enclave_ocall_sgx_fstat,
		(void*)Enclave_ocall_sgx_send,
		(void*)Enclave_ocall_sgx_recv,
		(void*)Enclave_ocall_sgx_sendto,
		(void*)Enclave_ocall_sgx_recvfrom,
		(void*)Enclave_ocall_sgx_gethostname,
		(void*)Enclave_ocall_sgx_getaddrinfo,
		(void*)Enclave_ocall_sgx_getenv,
		(void*)Enclave_ocall_sgx_getsockname,
		(void*)Enclave_ocall_sgx_getsockopt,
		(void*)Enclave_ocall_sgx_getservbyname,
		(void*)Enclave_ocall_sgx_getprotobynumber,
		(void*)Enclave_ocall_sgx_setsockopt,
		(void*)Enclave_ocall_sgx_htons,
		(void*)Enclave_ocall_sgx_htonl,
		(void*)Enclave_ocall_sgx_ntohs,
		(void*)Enclave_ocall_sgx_ntohl,
		(void*)Enclave_ocall_sgx_signal,
		(void*)Enclave_ocall_sgx_shutdown,
	}
};
sgx_status_t ecall_receive_binary(sgx_enclave_id_t eid, char* buffer, int sz)
{
	sgx_status_t status;
	ms_ecall_receive_binary_t ms;
	ms.ms_buffer = buffer;
	ms.ms_sz = sz;
	status = sgx_ecall(eid, 0, &ocall_table_Enclave, &ms);
	return status;
}

sgx_status_t ecall_receive_data(sgx_enclave_id_t eid, char* buffer, int sz)
{
	sgx_status_t status;
	ms_ecall_receive_data_t ms;
	ms.ms_buffer = buffer;
	ms.ms_sz = sz;
	status = sgx_ecall(eid, 1, &ocall_table_Enclave, &ms);
	return status;
}

