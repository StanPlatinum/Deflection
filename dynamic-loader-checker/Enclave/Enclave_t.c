#include "Enclave_t.h"

#include "sgx_trts.h" /* for sgx_ocalloc, sgx_is_outside_enclave */
#include "sgx_lfence.h" /* for sgx_lfence */

#include <errno.h>
#include <mbusafecrt.h> /* for memcpy_s etc */
#include <stdlib.h> /* for malloc/free etc */

#define CHECK_REF_POINTER(ptr, siz) do {	\
	if (!(ptr) || ! sgx_is_outside_enclave((ptr), (siz)))	\
		return SGX_ERROR_INVALID_PARAMETER;\
} while (0)

#define CHECK_UNIQUE_POINTER(ptr, siz) do {	\
	if ((ptr) && ! sgx_is_outside_enclave((ptr), (siz)))	\
		return SGX_ERROR_INVALID_PARAMETER;\
} while (0)

#define CHECK_ENCLAVE_POINTER(ptr, siz) do {	\
	if ((ptr) && ! sgx_is_within_enclave((ptr), (siz)))	\
		return SGX_ERROR_INVALID_PARAMETER;\
} while (0)

#define ADD_ASSIGN_OVERFLOW(a, b) (	\
	((a) += (b)) < (b)	\
)


typedef struct ms_ecall_receive_data_t {
	char* ms_buffer;
	int ms_sz;
} ms_ecall_receive_data_t;

typedef struct ms_ecall_receive_binary_t {
	char* ms_buffer;
	int ms_sz;
} ms_ecall_receive_binary_t;

typedef struct ms_ecall_receive_entrylabel_t {
	char* ms_buffer;
	int ms_sz;
} ms_ecall_receive_entrylabel_t;

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

typedef struct ms_ocall_sgx_puts1time_t {
	int ms_retval;
	char* ms_str;
} ms_ocall_sgx_puts1time_t;

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

static sgx_status_t SGX_CDECL sgx_ecall_receive_data(void* pms)
{
	CHECK_REF_POINTER(pms, sizeof(ms_ecall_receive_data_t));
	//
	// fence after pointer checks
	//
	sgx_lfence();
	ms_ecall_receive_data_t* ms = SGX_CAST(ms_ecall_receive_data_t*, pms);
	sgx_status_t status = SGX_SUCCESS;
	char* _tmp_buffer = ms->ms_buffer;



	ecall_receive_data(_tmp_buffer, ms->ms_sz);


	return status;
}

static sgx_status_t SGX_CDECL sgx_ecall_receive_binary(void* pms)
{
	CHECK_REF_POINTER(pms, sizeof(ms_ecall_receive_binary_t));
	//
	// fence after pointer checks
	//
	sgx_lfence();
	ms_ecall_receive_binary_t* ms = SGX_CAST(ms_ecall_receive_binary_t*, pms);
	sgx_status_t status = SGX_SUCCESS;
	char* _tmp_buffer = ms->ms_buffer;



	ecall_receive_binary(_tmp_buffer, ms->ms_sz);


	return status;
}

static sgx_status_t SGX_CDECL sgx_ecall_receive_entrylabel(void* pms)
{
	CHECK_REF_POINTER(pms, sizeof(ms_ecall_receive_entrylabel_t));
	//
	// fence after pointer checks
	//
	sgx_lfence();
	ms_ecall_receive_entrylabel_t* ms = SGX_CAST(ms_ecall_receive_entrylabel_t*, pms);
	sgx_status_t status = SGX_SUCCESS;
	char* _tmp_buffer = ms->ms_buffer;



	ecall_receive_entrylabel(_tmp_buffer, ms->ms_sz);


	return status;
}

SGX_EXTERNC const struct {
	size_t nr_ecall;
	struct {void* ecall_addr; uint8_t is_priv; uint8_t is_switchless;} ecall_table[3];
} g_ecall_table = {
	3,
	{
		{(void*)(uintptr_t)sgx_ecall_receive_data, 0, 0},
		{(void*)(uintptr_t)sgx_ecall_receive_binary, 0, 0},
		{(void*)(uintptr_t)sgx_ecall_receive_entrylabel, 0, 0},
	}
};

SGX_EXTERNC const struct {
	size_t nr_ocall;
	uint8_t entry_table[39][3];
} g_dyn_entry_table = {
	39,
	{
		{0, 0, 0, },
		{0, 0, 0, },
		{0, 0, 0, },
		{0, 0, 0, },
		{0, 0, 0, },
		{0, 0, 0, },
		{0, 0, 0, },
		{0, 0, 0, },
		{0, 0, 0, },
		{0, 0, 0, },
		{0, 0, 0, },
		{0, 0, 0, },
		{0, 0, 0, },
		{0, 0, 0, },
		{0, 0, 0, },
		{0, 0, 0, },
		{0, 0, 0, },
		{0, 0, 0, },
		{0, 0, 0, },
		{0, 0, 0, },
		{0, 0, 0, },
		{0, 0, 0, },
		{0, 0, 0, },
		{0, 0, 0, },
		{0, 0, 0, },
		{0, 0, 0, },
		{0, 0, 0, },
		{0, 0, 0, },
		{0, 0, 0, },
		{0, 0, 0, },
		{0, 0, 0, },
		{0, 0, 0, },
		{0, 0, 0, },
		{0, 0, 0, },
		{0, 0, 0, },
		{0, 0, 0, },
		{0, 0, 0, },
		{0, 0, 0, },
		{0, 0, 0, },
	}
};


sgx_status_t SGX_CDECL ocall_print_string(const char* str)
{
	sgx_status_t status = SGX_SUCCESS;
	size_t _len_str = str ? strlen(str) + 1 : 0;

	ms_ocall_print_string_t* ms = NULL;
	size_t ocalloc_size = sizeof(ms_ocall_print_string_t);
	void *__tmp = NULL;


	CHECK_ENCLAVE_POINTER(str, _len_str);

	if (ADD_ASSIGN_OVERFLOW(ocalloc_size, (str != NULL) ? _len_str : 0))
		return SGX_ERROR_INVALID_PARAMETER;

	__tmp = sgx_ocalloc(ocalloc_size);
	if (__tmp == NULL) {
		sgx_ocfree();
		return SGX_ERROR_UNEXPECTED;
	}
	ms = (ms_ocall_print_string_t*)__tmp;
	__tmp = (void *)((size_t)__tmp + sizeof(ms_ocall_print_string_t));
	ocalloc_size -= sizeof(ms_ocall_print_string_t);

	if (str != NULL) {
		ms->ms_str = (const char*)__tmp;
		if (_len_str % sizeof(*str) != 0) {
			sgx_ocfree();
			return SGX_ERROR_INVALID_PARAMETER;
		}
		if (memcpy_s(__tmp, ocalloc_size, str, _len_str)) {
			sgx_ocfree();
			return SGX_ERROR_UNEXPECTED;
		}
		__tmp = (void *)((size_t)__tmp + _len_str);
		ocalloc_size -= _len_str;
	} else {
		ms->ms_str = NULL;
	}
	
	status = sgx_ocall(0, ms);

	if (status == SGX_SUCCESS) {
	}
	sgx_ocfree();
	return status;
}

sgx_status_t SGX_CDECL ocall_sgx_push_gadget(unsigned long int gadget)
{
	sgx_status_t status = SGX_SUCCESS;

	ms_ocall_sgx_push_gadget_t* ms = NULL;
	size_t ocalloc_size = sizeof(ms_ocall_sgx_push_gadget_t);
	void *__tmp = NULL;


	__tmp = sgx_ocalloc(ocalloc_size);
	if (__tmp == NULL) {
		sgx_ocfree();
		return SGX_ERROR_UNEXPECTED;
	}
	ms = (ms_ocall_sgx_push_gadget_t*)__tmp;
	__tmp = (void *)((size_t)__tmp + sizeof(ms_ocall_sgx_push_gadget_t));
	ocalloc_size -= sizeof(ms_ocall_sgx_push_gadget_t);

	ms->ms_gadget = gadget;
	status = sgx_ocall(1, ms);

	if (status == SGX_SUCCESS) {
	}
	sgx_ocfree();
	return status;
}

sgx_status_t SGX_CDECL ocall_sgx_clock(clock_t* retval)
{
	sgx_status_t status = SGX_SUCCESS;

	ms_ocall_sgx_clock_t* ms = NULL;
	size_t ocalloc_size = sizeof(ms_ocall_sgx_clock_t);
	void *__tmp = NULL;


	__tmp = sgx_ocalloc(ocalloc_size);
	if (__tmp == NULL) {
		sgx_ocfree();
		return SGX_ERROR_UNEXPECTED;
	}
	ms = (ms_ocall_sgx_clock_t*)__tmp;
	__tmp = (void *)((size_t)__tmp + sizeof(ms_ocall_sgx_clock_t));
	ocalloc_size -= sizeof(ms_ocall_sgx_clock_t);

	status = sgx_ocall(2, ms);

	if (status == SGX_SUCCESS) {
		if (retval) *retval = ms->ms_retval;
	}
	sgx_ocfree();
	return status;
}

sgx_status_t SGX_CDECL ocall_sgx_time(time_t* retval, time_t* t)
{
	sgx_status_t status = SGX_SUCCESS;
	size_t _len_t = 8;

	ms_ocall_sgx_time_t* ms = NULL;
	size_t ocalloc_size = sizeof(ms_ocall_sgx_time_t);
	void *__tmp = NULL;

	void *__tmp_t = NULL;

	CHECK_ENCLAVE_POINTER(t, _len_t);

	if (ADD_ASSIGN_OVERFLOW(ocalloc_size, (t != NULL) ? _len_t : 0))
		return SGX_ERROR_INVALID_PARAMETER;

	__tmp = sgx_ocalloc(ocalloc_size);
	if (__tmp == NULL) {
		sgx_ocfree();
		return SGX_ERROR_UNEXPECTED;
	}
	ms = (ms_ocall_sgx_time_t*)__tmp;
	__tmp = (void *)((size_t)__tmp + sizeof(ms_ocall_sgx_time_t));
	ocalloc_size -= sizeof(ms_ocall_sgx_time_t);

	if (t != NULL) {
		ms->ms_t = (time_t*)__tmp;
		__tmp_t = __tmp;
		if (memcpy_s(__tmp, ocalloc_size, t, _len_t)) {
			sgx_ocfree();
			return SGX_ERROR_UNEXPECTED;
		}
		__tmp = (void *)((size_t)__tmp + _len_t);
		ocalloc_size -= _len_t;
	} else {
		ms->ms_t = NULL;
	}
	
	status = sgx_ocall(3, ms);

	if (status == SGX_SUCCESS) {
		if (retval) *retval = ms->ms_retval;
		if (t) {
			if (memcpy_s((void*)t, _len_t, __tmp_t, _len_t)) {
				sgx_ocfree();
				return SGX_ERROR_UNEXPECTED;
			}
		}
	}
	sgx_ocfree();
	return status;
}

sgx_status_t SGX_CDECL ocall_sgx_localtime(struct tm** retval, const time_t* timep)
{
	sgx_status_t status = SGX_SUCCESS;
	size_t _len_timep = 8;

	ms_ocall_sgx_localtime_t* ms = NULL;
	size_t ocalloc_size = sizeof(ms_ocall_sgx_localtime_t);
	void *__tmp = NULL;


	CHECK_ENCLAVE_POINTER(timep, _len_timep);

	if (ADD_ASSIGN_OVERFLOW(ocalloc_size, (timep != NULL) ? _len_timep : 0))
		return SGX_ERROR_INVALID_PARAMETER;

	__tmp = sgx_ocalloc(ocalloc_size);
	if (__tmp == NULL) {
		sgx_ocfree();
		return SGX_ERROR_UNEXPECTED;
	}
	ms = (ms_ocall_sgx_localtime_t*)__tmp;
	__tmp = (void *)((size_t)__tmp + sizeof(ms_ocall_sgx_localtime_t));
	ocalloc_size -= sizeof(ms_ocall_sgx_localtime_t);

	if (timep != NULL) {
		ms->ms_timep = (const time_t*)__tmp;
		if (memcpy_s(__tmp, ocalloc_size, timep, _len_timep)) {
			sgx_ocfree();
			return SGX_ERROR_UNEXPECTED;
		}
		__tmp = (void *)((size_t)__tmp + _len_timep);
		ocalloc_size -= _len_timep;
	} else {
		ms->ms_timep = NULL;
	}
	
	status = sgx_ocall(4, ms);

	if (status == SGX_SUCCESS) {
		if (retval) *retval = ms->ms_retval;
	}
	sgx_ocfree();
	return status;
}

sgx_status_t SGX_CDECL ocall_sgx_gmtime(struct tm** retval, const time_t* timep)
{
	sgx_status_t status = SGX_SUCCESS;
	size_t _len_timep = 8;

	ms_ocall_sgx_gmtime_t* ms = NULL;
	size_t ocalloc_size = sizeof(ms_ocall_sgx_gmtime_t);
	void *__tmp = NULL;


	CHECK_ENCLAVE_POINTER(timep, _len_timep);

	if (ADD_ASSIGN_OVERFLOW(ocalloc_size, (timep != NULL) ? _len_timep : 0))
		return SGX_ERROR_INVALID_PARAMETER;

	__tmp = sgx_ocalloc(ocalloc_size);
	if (__tmp == NULL) {
		sgx_ocfree();
		return SGX_ERROR_UNEXPECTED;
	}
	ms = (ms_ocall_sgx_gmtime_t*)__tmp;
	__tmp = (void *)((size_t)__tmp + sizeof(ms_ocall_sgx_gmtime_t));
	ocalloc_size -= sizeof(ms_ocall_sgx_gmtime_t);

	if (timep != NULL) {
		ms->ms_timep = (const time_t*)__tmp;
		if (memcpy_s(__tmp, ocalloc_size, timep, _len_timep)) {
			sgx_ocfree();
			return SGX_ERROR_UNEXPECTED;
		}
		__tmp = (void *)((size_t)__tmp + _len_timep);
		ocalloc_size -= _len_timep;
	} else {
		ms->ms_timep = NULL;
	}
	
	status = sgx_ocall(5, ms);

	if (status == SGX_SUCCESS) {
		if (retval) *retval = ms->ms_retval;
	}
	sgx_ocfree();
	return status;
}

sgx_status_t SGX_CDECL ocall_sgx_mktime(time_t* retval, struct tm* tmptr)
{
	sgx_status_t status = SGX_SUCCESS;
	size_t _len_tmptr = 56;

	ms_ocall_sgx_mktime_t* ms = NULL;
	size_t ocalloc_size = sizeof(ms_ocall_sgx_mktime_t);
	void *__tmp = NULL;

	void *__tmp_tmptr = NULL;

	CHECK_ENCLAVE_POINTER(tmptr, _len_tmptr);

	if (ADD_ASSIGN_OVERFLOW(ocalloc_size, (tmptr != NULL) ? _len_tmptr : 0))
		return SGX_ERROR_INVALID_PARAMETER;

	__tmp = sgx_ocalloc(ocalloc_size);
	if (__tmp == NULL) {
		sgx_ocfree();
		return SGX_ERROR_UNEXPECTED;
	}
	ms = (ms_ocall_sgx_mktime_t*)__tmp;
	__tmp = (void *)((size_t)__tmp + sizeof(ms_ocall_sgx_mktime_t));
	ocalloc_size -= sizeof(ms_ocall_sgx_mktime_t);

	if (tmptr != NULL) {
		ms->ms_tmptr = (struct tm*)__tmp;
		__tmp_tmptr = __tmp;
		memset(__tmp_tmptr, 0, _len_tmptr);
		__tmp = (void *)((size_t)__tmp + _len_tmptr);
		ocalloc_size -= _len_tmptr;
	} else {
		ms->ms_tmptr = NULL;
	}
	
	status = sgx_ocall(6, ms);

	if (status == SGX_SUCCESS) {
		if (retval) *retval = ms->ms_retval;
		if (tmptr) {
			if (memcpy_s((void*)tmptr, _len_tmptr, __tmp_tmptr, _len_tmptr)) {
				sgx_ocfree();
				return SGX_ERROR_UNEXPECTED;
			}
		}
	}
	sgx_ocfree();
	return status;
}

sgx_status_t SGX_CDECL ocall_sgx_gettimeofday(int* retval, struct timeval* tv, struct timezone* tz)
{
	sgx_status_t status = SGX_SUCCESS;
	size_t _len_tv = 16;
	size_t _len_tz = 8;

	ms_ocall_sgx_gettimeofday_t* ms = NULL;
	size_t ocalloc_size = sizeof(ms_ocall_sgx_gettimeofday_t);
	void *__tmp = NULL;

	void *__tmp_tv = NULL;
	void *__tmp_tz = NULL;

	CHECK_ENCLAVE_POINTER(tv, _len_tv);
	CHECK_ENCLAVE_POINTER(tz, _len_tz);

	if (ADD_ASSIGN_OVERFLOW(ocalloc_size, (tv != NULL) ? _len_tv : 0))
		return SGX_ERROR_INVALID_PARAMETER;
	if (ADD_ASSIGN_OVERFLOW(ocalloc_size, (tz != NULL) ? _len_tz : 0))
		return SGX_ERROR_INVALID_PARAMETER;

	__tmp = sgx_ocalloc(ocalloc_size);
	if (__tmp == NULL) {
		sgx_ocfree();
		return SGX_ERROR_UNEXPECTED;
	}
	ms = (ms_ocall_sgx_gettimeofday_t*)__tmp;
	__tmp = (void *)((size_t)__tmp + sizeof(ms_ocall_sgx_gettimeofday_t));
	ocalloc_size -= sizeof(ms_ocall_sgx_gettimeofday_t);

	if (tv != NULL) {
		ms->ms_tv = (struct timeval*)__tmp;
		__tmp_tv = __tmp;
		memset(__tmp_tv, 0, _len_tv);
		__tmp = (void *)((size_t)__tmp + _len_tv);
		ocalloc_size -= _len_tv;
	} else {
		ms->ms_tv = NULL;
	}
	
	if (tz != NULL) {
		ms->ms_tz = (struct timezone*)__tmp;
		__tmp_tz = __tmp;
		memset(__tmp_tz, 0, _len_tz);
		__tmp = (void *)((size_t)__tmp + _len_tz);
		ocalloc_size -= _len_tz;
	} else {
		ms->ms_tz = NULL;
	}
	
	status = sgx_ocall(7, ms);

	if (status == SGX_SUCCESS) {
		if (retval) *retval = ms->ms_retval;
		if (tv) {
			if (memcpy_s((void*)tv, _len_tv, __tmp_tv, _len_tv)) {
				sgx_ocfree();
				return SGX_ERROR_UNEXPECTED;
			}
		}
		if (tz) {
			if (memcpy_s((void*)tz, _len_tz, __tmp_tz, _len_tz)) {
				sgx_ocfree();
				return SGX_ERROR_UNEXPECTED;
			}
		}
	}
	sgx_ocfree();
	return status;
}

sgx_status_t SGX_CDECL ocall_sgx_puts1time(int* retval, char* str)
{
	sgx_status_t status = SGX_SUCCESS;
	size_t _len_str = 80;

	ms_ocall_sgx_puts1time_t* ms = NULL;
	size_t ocalloc_size = sizeof(ms_ocall_sgx_puts1time_t);
	void *__tmp = NULL;


	CHECK_ENCLAVE_POINTER(str, _len_str);

	if (ADD_ASSIGN_OVERFLOW(ocalloc_size, (str != NULL) ? _len_str : 0))
		return SGX_ERROR_INVALID_PARAMETER;

	__tmp = sgx_ocalloc(ocalloc_size);
	if (__tmp == NULL) {
		sgx_ocfree();
		return SGX_ERROR_UNEXPECTED;
	}
	ms = (ms_ocall_sgx_puts1time_t*)__tmp;
	__tmp = (void *)((size_t)__tmp + sizeof(ms_ocall_sgx_puts1time_t));
	ocalloc_size -= sizeof(ms_ocall_sgx_puts1time_t);

	if (str != NULL) {
		ms->ms_str = (char*)__tmp;
		if (_len_str % sizeof(*str) != 0) {
			sgx_ocfree();
			return SGX_ERROR_INVALID_PARAMETER;
		}
		if (memcpy_s(__tmp, ocalloc_size, str, _len_str)) {
			sgx_ocfree();
			return SGX_ERROR_UNEXPECTED;
		}
		__tmp = (void *)((size_t)__tmp + _len_str);
		ocalloc_size -= _len_str;
	} else {
		ms->ms_str = NULL;
	}
	
	status = sgx_ocall(8, ms);

	if (status == SGX_SUCCESS) {
		if (retval) *retval = ms->ms_retval;
	}
	sgx_ocfree();
	return status;
}

sgx_status_t SGX_CDECL ocall_sgx_puts(int* retval, const char* str)
{
	sgx_status_t status = SGX_SUCCESS;
	size_t _len_str = str ? strlen(str) + 1 : 0;

	ms_ocall_sgx_puts_t* ms = NULL;
	size_t ocalloc_size = sizeof(ms_ocall_sgx_puts_t);
	void *__tmp = NULL;


	CHECK_ENCLAVE_POINTER(str, _len_str);

	if (ADD_ASSIGN_OVERFLOW(ocalloc_size, (str != NULL) ? _len_str : 0))
		return SGX_ERROR_INVALID_PARAMETER;

	__tmp = sgx_ocalloc(ocalloc_size);
	if (__tmp == NULL) {
		sgx_ocfree();
		return SGX_ERROR_UNEXPECTED;
	}
	ms = (ms_ocall_sgx_puts_t*)__tmp;
	__tmp = (void *)((size_t)__tmp + sizeof(ms_ocall_sgx_puts_t));
	ocalloc_size -= sizeof(ms_ocall_sgx_puts_t);

	if (str != NULL) {
		ms->ms_str = (const char*)__tmp;
		if (_len_str % sizeof(*str) != 0) {
			sgx_ocfree();
			return SGX_ERROR_INVALID_PARAMETER;
		}
		if (memcpy_s(__tmp, ocalloc_size, str, _len_str)) {
			sgx_ocfree();
			return SGX_ERROR_UNEXPECTED;
		}
		__tmp = (void *)((size_t)__tmp + _len_str);
		ocalloc_size -= _len_str;
	} else {
		ms->ms_str = NULL;
	}
	
	status = sgx_ocall(9, ms);

	if (status == SGX_SUCCESS) {
		if (retval) *retval = ms->ms_retval;
	}
	sgx_ocfree();
	return status;
}

sgx_status_t SGX_CDECL ocall_sgx_open(int* retval, const char* pathname, int flags, unsigned int mode)
{
	sgx_status_t status = SGX_SUCCESS;
	size_t _len_pathname = pathname ? strlen(pathname) + 1 : 0;

	ms_ocall_sgx_open_t* ms = NULL;
	size_t ocalloc_size = sizeof(ms_ocall_sgx_open_t);
	void *__tmp = NULL;


	CHECK_ENCLAVE_POINTER(pathname, _len_pathname);

	if (ADD_ASSIGN_OVERFLOW(ocalloc_size, (pathname != NULL) ? _len_pathname : 0))
		return SGX_ERROR_INVALID_PARAMETER;

	__tmp = sgx_ocalloc(ocalloc_size);
	if (__tmp == NULL) {
		sgx_ocfree();
		return SGX_ERROR_UNEXPECTED;
	}
	ms = (ms_ocall_sgx_open_t*)__tmp;
	__tmp = (void *)((size_t)__tmp + sizeof(ms_ocall_sgx_open_t));
	ocalloc_size -= sizeof(ms_ocall_sgx_open_t);

	if (pathname != NULL) {
		ms->ms_pathname = (const char*)__tmp;
		if (_len_pathname % sizeof(*pathname) != 0) {
			sgx_ocfree();
			return SGX_ERROR_INVALID_PARAMETER;
		}
		if (memcpy_s(__tmp, ocalloc_size, pathname, _len_pathname)) {
			sgx_ocfree();
			return SGX_ERROR_UNEXPECTED;
		}
		__tmp = (void *)((size_t)__tmp + _len_pathname);
		ocalloc_size -= _len_pathname;
	} else {
		ms->ms_pathname = NULL;
	}
	
	ms->ms_flags = flags;
	ms->ms_mode = mode;
	status = sgx_ocall(10, ms);

	if (status == SGX_SUCCESS) {
		if (retval) *retval = ms->ms_retval;
	}
	sgx_ocfree();
	return status;
}

sgx_status_t SGX_CDECL ocall_sgx_close(int* retval, int fd)
{
	sgx_status_t status = SGX_SUCCESS;

	ms_ocall_sgx_close_t* ms = NULL;
	size_t ocalloc_size = sizeof(ms_ocall_sgx_close_t);
	void *__tmp = NULL;


	__tmp = sgx_ocalloc(ocalloc_size);
	if (__tmp == NULL) {
		sgx_ocfree();
		return SGX_ERROR_UNEXPECTED;
	}
	ms = (ms_ocall_sgx_close_t*)__tmp;
	__tmp = (void *)((size_t)__tmp + sizeof(ms_ocall_sgx_close_t));
	ocalloc_size -= sizeof(ms_ocall_sgx_close_t);

	ms->ms_fd = fd;
	status = sgx_ocall(11, ms);

	if (status == SGX_SUCCESS) {
		if (retval) *retval = ms->ms_retval;
	}
	sgx_ocfree();
	return status;
}

sgx_status_t SGX_CDECL ocall_sgx_read(ssize_t* retval, int fd, char* buf, size_t count)
{
	sgx_status_t status = SGX_SUCCESS;
	size_t _len_buf = count;

	ms_ocall_sgx_read_t* ms = NULL;
	size_t ocalloc_size = sizeof(ms_ocall_sgx_read_t);
	void *__tmp = NULL;

	void *__tmp_buf = NULL;

	CHECK_ENCLAVE_POINTER(buf, _len_buf);

	if (ADD_ASSIGN_OVERFLOW(ocalloc_size, (buf != NULL) ? _len_buf : 0))
		return SGX_ERROR_INVALID_PARAMETER;

	__tmp = sgx_ocalloc(ocalloc_size);
	if (__tmp == NULL) {
		sgx_ocfree();
		return SGX_ERROR_UNEXPECTED;
	}
	ms = (ms_ocall_sgx_read_t*)__tmp;
	__tmp = (void *)((size_t)__tmp + sizeof(ms_ocall_sgx_read_t));
	ocalloc_size -= sizeof(ms_ocall_sgx_read_t);

	ms->ms_fd = fd;
	if (buf != NULL) {
		ms->ms_buf = (char*)__tmp;
		__tmp_buf = __tmp;
		if (_len_buf % sizeof(*buf) != 0) {
			sgx_ocfree();
			return SGX_ERROR_INVALID_PARAMETER;
		}
		memset(__tmp_buf, 0, _len_buf);
		__tmp = (void *)((size_t)__tmp + _len_buf);
		ocalloc_size -= _len_buf;
	} else {
		ms->ms_buf = NULL;
	}
	
	ms->ms_count = count;
	status = sgx_ocall(12, ms);

	if (status == SGX_SUCCESS) {
		if (retval) *retval = ms->ms_retval;
		if (buf) {
			if (memcpy_s((void*)buf, _len_buf, __tmp_buf, _len_buf)) {
				sgx_ocfree();
				return SGX_ERROR_UNEXPECTED;
			}
		}
	}
	sgx_ocfree();
	return status;
}

sgx_status_t SGX_CDECL ocall_sgx_write(ssize_t* retval, int fd, const char* buf, size_t count)
{
	sgx_status_t status = SGX_SUCCESS;
	size_t _len_buf = count;

	ms_ocall_sgx_write_t* ms = NULL;
	size_t ocalloc_size = sizeof(ms_ocall_sgx_write_t);
	void *__tmp = NULL;


	CHECK_ENCLAVE_POINTER(buf, _len_buf);

	if (ADD_ASSIGN_OVERFLOW(ocalloc_size, (buf != NULL) ? _len_buf : 0))
		return SGX_ERROR_INVALID_PARAMETER;

	__tmp = sgx_ocalloc(ocalloc_size);
	if (__tmp == NULL) {
		sgx_ocfree();
		return SGX_ERROR_UNEXPECTED;
	}
	ms = (ms_ocall_sgx_write_t*)__tmp;
	__tmp = (void *)((size_t)__tmp + sizeof(ms_ocall_sgx_write_t));
	ocalloc_size -= sizeof(ms_ocall_sgx_write_t);

	ms->ms_fd = fd;
	if (buf != NULL) {
		ms->ms_buf = (const char*)__tmp;
		if (_len_buf % sizeof(*buf) != 0) {
			sgx_ocfree();
			return SGX_ERROR_INVALID_PARAMETER;
		}
		if (memcpy_s(__tmp, ocalloc_size, buf, _len_buf)) {
			sgx_ocfree();
			return SGX_ERROR_UNEXPECTED;
		}
		__tmp = (void *)((size_t)__tmp + _len_buf);
		ocalloc_size -= _len_buf;
	} else {
		ms->ms_buf = NULL;
	}
	
	ms->ms_count = count;
	status = sgx_ocall(13, ms);

	if (status == SGX_SUCCESS) {
		if (retval) *retval = ms->ms_retval;
	}
	sgx_ocfree();
	return status;
}

sgx_status_t SGX_CDECL ocall_sgx_lseek(off_t* retval, int fildes, off_t offset, int whence)
{
	sgx_status_t status = SGX_SUCCESS;

	ms_ocall_sgx_lseek_t* ms = NULL;
	size_t ocalloc_size = sizeof(ms_ocall_sgx_lseek_t);
	void *__tmp = NULL;


	__tmp = sgx_ocalloc(ocalloc_size);
	if (__tmp == NULL) {
		sgx_ocfree();
		return SGX_ERROR_UNEXPECTED;
	}
	ms = (ms_ocall_sgx_lseek_t*)__tmp;
	__tmp = (void *)((size_t)__tmp + sizeof(ms_ocall_sgx_lseek_t));
	ocalloc_size -= sizeof(ms_ocall_sgx_lseek_t);

	ms->ms_fildes = fildes;
	ms->ms_offset = offset;
	ms->ms_whence = whence;
	status = sgx_ocall(14, ms);

	if (status == SGX_SUCCESS) {
		if (retval) *retval = ms->ms_retval;
	}
	sgx_ocfree();
	return status;
}

sgx_status_t SGX_CDECL ocall_sgx_socket(int* retval, int af, int type, int protocol)
{
	sgx_status_t status = SGX_SUCCESS;

	ms_ocall_sgx_socket_t* ms = NULL;
	size_t ocalloc_size = sizeof(ms_ocall_sgx_socket_t);
	void *__tmp = NULL;


	__tmp = sgx_ocalloc(ocalloc_size);
	if (__tmp == NULL) {
		sgx_ocfree();
		return SGX_ERROR_UNEXPECTED;
	}
	ms = (ms_ocall_sgx_socket_t*)__tmp;
	__tmp = (void *)((size_t)__tmp + sizeof(ms_ocall_sgx_socket_t));
	ocalloc_size -= sizeof(ms_ocall_sgx_socket_t);

	ms->ms_af = af;
	ms->ms_type = type;
	ms->ms_protocol = protocol;
	status = sgx_ocall(15, ms);

	if (status == SGX_SUCCESS) {
		if (retval) *retval = ms->ms_retval;
	}
	sgx_ocfree();
	return status;
}

sgx_status_t SGX_CDECL ocall_sgx_bind(int* retval, int s, unsigned long int addr, size_t addr_size)
{
	sgx_status_t status = SGX_SUCCESS;

	ms_ocall_sgx_bind_t* ms = NULL;
	size_t ocalloc_size = sizeof(ms_ocall_sgx_bind_t);
	void *__tmp = NULL;


	__tmp = sgx_ocalloc(ocalloc_size);
	if (__tmp == NULL) {
		sgx_ocfree();
		return SGX_ERROR_UNEXPECTED;
	}
	ms = (ms_ocall_sgx_bind_t*)__tmp;
	__tmp = (void *)((size_t)__tmp + sizeof(ms_ocall_sgx_bind_t));
	ocalloc_size -= sizeof(ms_ocall_sgx_bind_t);

	ms->ms_s = s;
	ms->ms_addr = addr;
	ms->ms_addr_size = addr_size;
	status = sgx_ocall(16, ms);

	if (status == SGX_SUCCESS) {
		if (retval) *retval = ms->ms_retval;
	}
	sgx_ocfree();
	return status;
}

sgx_status_t SGX_CDECL ocall_sgx_connect(int* retval, int s, const struct sockaddr* addr, size_t addrlen)
{
	sgx_status_t status = SGX_SUCCESS;
	size_t _len_addr = addrlen;

	ms_ocall_sgx_connect_t* ms = NULL;
	size_t ocalloc_size = sizeof(ms_ocall_sgx_connect_t);
	void *__tmp = NULL;


	CHECK_ENCLAVE_POINTER(addr, _len_addr);

	if (ADD_ASSIGN_OVERFLOW(ocalloc_size, (addr != NULL) ? _len_addr : 0))
		return SGX_ERROR_INVALID_PARAMETER;

	__tmp = sgx_ocalloc(ocalloc_size);
	if (__tmp == NULL) {
		sgx_ocfree();
		return SGX_ERROR_UNEXPECTED;
	}
	ms = (ms_ocall_sgx_connect_t*)__tmp;
	__tmp = (void *)((size_t)__tmp + sizeof(ms_ocall_sgx_connect_t));
	ocalloc_size -= sizeof(ms_ocall_sgx_connect_t);

	ms->ms_s = s;
	if (addr != NULL) {
		ms->ms_addr = (const struct sockaddr*)__tmp;
		if (memcpy_s(__tmp, ocalloc_size, addr, _len_addr)) {
			sgx_ocfree();
			return SGX_ERROR_UNEXPECTED;
		}
		__tmp = (void *)((size_t)__tmp + _len_addr);
		ocalloc_size -= _len_addr;
	} else {
		ms->ms_addr = NULL;
	}
	
	ms->ms_addrlen = addrlen;
	status = sgx_ocall(17, ms);

	if (status == SGX_SUCCESS) {
		if (retval) *retval = ms->ms_retval;
	}
	sgx_ocfree();
	return status;
}

sgx_status_t SGX_CDECL ocall_sgx_listen(int* retval, int s, int backlog)
{
	sgx_status_t status = SGX_SUCCESS;

	ms_ocall_sgx_listen_t* ms = NULL;
	size_t ocalloc_size = sizeof(ms_ocall_sgx_listen_t);
	void *__tmp = NULL;


	__tmp = sgx_ocalloc(ocalloc_size);
	if (__tmp == NULL) {
		sgx_ocfree();
		return SGX_ERROR_UNEXPECTED;
	}
	ms = (ms_ocall_sgx_listen_t*)__tmp;
	__tmp = (void *)((size_t)__tmp + sizeof(ms_ocall_sgx_listen_t));
	ocalloc_size -= sizeof(ms_ocall_sgx_listen_t);

	ms->ms_s = s;
	ms->ms_backlog = backlog;
	status = sgx_ocall(18, ms);

	if (status == SGX_SUCCESS) {
		if (retval) *retval = ms->ms_retval;
	}
	sgx_ocfree();
	return status;
}

sgx_status_t SGX_CDECL ocall_sgx_accept(int* retval, int s, struct sockaddr* addr, unsigned int addr_size, socklen_t* addrlen)
{
	sgx_status_t status = SGX_SUCCESS;
	size_t _len_addr = addr_size;
	size_t _len_addrlen = sizeof(socklen_t);

	ms_ocall_sgx_accept_t* ms = NULL;
	size_t ocalloc_size = sizeof(ms_ocall_sgx_accept_t);
	void *__tmp = NULL;

	void *__tmp_addr = NULL;
	void *__tmp_addrlen = NULL;

	CHECK_ENCLAVE_POINTER(addr, _len_addr);
	CHECK_ENCLAVE_POINTER(addrlen, _len_addrlen);

	if (ADD_ASSIGN_OVERFLOW(ocalloc_size, (addr != NULL) ? _len_addr : 0))
		return SGX_ERROR_INVALID_PARAMETER;
	if (ADD_ASSIGN_OVERFLOW(ocalloc_size, (addrlen != NULL) ? _len_addrlen : 0))
		return SGX_ERROR_INVALID_PARAMETER;

	__tmp = sgx_ocalloc(ocalloc_size);
	if (__tmp == NULL) {
		sgx_ocfree();
		return SGX_ERROR_UNEXPECTED;
	}
	ms = (ms_ocall_sgx_accept_t*)__tmp;
	__tmp = (void *)((size_t)__tmp + sizeof(ms_ocall_sgx_accept_t));
	ocalloc_size -= sizeof(ms_ocall_sgx_accept_t);

	ms->ms_s = s;
	if (addr != NULL) {
		ms->ms_addr = (struct sockaddr*)__tmp;
		__tmp_addr = __tmp;
		memset(__tmp_addr, 0, _len_addr);
		__tmp = (void *)((size_t)__tmp + _len_addr);
		ocalloc_size -= _len_addr;
	} else {
		ms->ms_addr = NULL;
	}
	
	ms->ms_addr_size = addr_size;
	if (addrlen != NULL) {
		ms->ms_addrlen = (socklen_t*)__tmp;
		__tmp_addrlen = __tmp;
		memset(__tmp_addrlen, 0, _len_addrlen);
		__tmp = (void *)((size_t)__tmp + _len_addrlen);
		ocalloc_size -= _len_addrlen;
	} else {
		ms->ms_addrlen = NULL;
	}
	
	status = sgx_ocall(19, ms);

	if (status == SGX_SUCCESS) {
		if (retval) *retval = ms->ms_retval;
		if (addr) {
			if (memcpy_s((void*)addr, _len_addr, __tmp_addr, _len_addr)) {
				sgx_ocfree();
				return SGX_ERROR_UNEXPECTED;
			}
		}
		if (addrlen) {
			if (memcpy_s((void*)addrlen, _len_addrlen, __tmp_addrlen, _len_addrlen)) {
				sgx_ocfree();
				return SGX_ERROR_UNEXPECTED;
			}
		}
	}
	sgx_ocfree();
	return status;
}

sgx_status_t SGX_CDECL ocall_sgx_fstat(int* retval, int fd, struct stat* buf)
{
	sgx_status_t status = SGX_SUCCESS;
	size_t _len_buf = 144;

	ms_ocall_sgx_fstat_t* ms = NULL;
	size_t ocalloc_size = sizeof(ms_ocall_sgx_fstat_t);
	void *__tmp = NULL;

	void *__tmp_buf = NULL;

	CHECK_ENCLAVE_POINTER(buf, _len_buf);

	if (ADD_ASSIGN_OVERFLOW(ocalloc_size, (buf != NULL) ? _len_buf : 0))
		return SGX_ERROR_INVALID_PARAMETER;

	__tmp = sgx_ocalloc(ocalloc_size);
	if (__tmp == NULL) {
		sgx_ocfree();
		return SGX_ERROR_UNEXPECTED;
	}
	ms = (ms_ocall_sgx_fstat_t*)__tmp;
	__tmp = (void *)((size_t)__tmp + sizeof(ms_ocall_sgx_fstat_t));
	ocalloc_size -= sizeof(ms_ocall_sgx_fstat_t);

	ms->ms_fd = fd;
	if (buf != NULL) {
		ms->ms_buf = (struct stat*)__tmp;
		__tmp_buf = __tmp;
		memset(__tmp_buf, 0, _len_buf);
		__tmp = (void *)((size_t)__tmp + _len_buf);
		ocalloc_size -= _len_buf;
	} else {
		ms->ms_buf = NULL;
	}
	
	status = sgx_ocall(20, ms);

	if (status == SGX_SUCCESS) {
		if (retval) *retval = ms->ms_retval;
		if (buf) {
			if (memcpy_s((void*)buf, _len_buf, __tmp_buf, _len_buf)) {
				sgx_ocfree();
				return SGX_ERROR_UNEXPECTED;
			}
		}
	}
	sgx_ocfree();
	return status;
}

sgx_status_t SGX_CDECL ocall_sgx_send(ssize_t* retval, int s, const void* buf, size_t len, int flags)
{
	sgx_status_t status = SGX_SUCCESS;
	size_t _len_buf = len;

	ms_ocall_sgx_send_t* ms = NULL;
	size_t ocalloc_size = sizeof(ms_ocall_sgx_send_t);
	void *__tmp = NULL;


	CHECK_ENCLAVE_POINTER(buf, _len_buf);

	if (ADD_ASSIGN_OVERFLOW(ocalloc_size, (buf != NULL) ? _len_buf : 0))
		return SGX_ERROR_INVALID_PARAMETER;

	__tmp = sgx_ocalloc(ocalloc_size);
	if (__tmp == NULL) {
		sgx_ocfree();
		return SGX_ERROR_UNEXPECTED;
	}
	ms = (ms_ocall_sgx_send_t*)__tmp;
	__tmp = (void *)((size_t)__tmp + sizeof(ms_ocall_sgx_send_t));
	ocalloc_size -= sizeof(ms_ocall_sgx_send_t);

	ms->ms_s = s;
	if (buf != NULL) {
		ms->ms_buf = (const void*)__tmp;
		if (memcpy_s(__tmp, ocalloc_size, buf, _len_buf)) {
			sgx_ocfree();
			return SGX_ERROR_UNEXPECTED;
		}
		__tmp = (void *)((size_t)__tmp + _len_buf);
		ocalloc_size -= _len_buf;
	} else {
		ms->ms_buf = NULL;
	}
	
	ms->ms_len = len;
	ms->ms_flags = flags;
	status = sgx_ocall(21, ms);

	if (status == SGX_SUCCESS) {
		if (retval) *retval = ms->ms_retval;
	}
	sgx_ocfree();
	return status;
}

sgx_status_t SGX_CDECL ocall_sgx_recv(ssize_t* retval, int s, void* buf, size_t len, int flags)
{
	sgx_status_t status = SGX_SUCCESS;
	size_t _len_buf = len;

	ms_ocall_sgx_recv_t* ms = NULL;
	size_t ocalloc_size = sizeof(ms_ocall_sgx_recv_t);
	void *__tmp = NULL;

	void *__tmp_buf = NULL;

	CHECK_ENCLAVE_POINTER(buf, _len_buf);

	if (ADD_ASSIGN_OVERFLOW(ocalloc_size, (buf != NULL) ? _len_buf : 0))
		return SGX_ERROR_INVALID_PARAMETER;

	__tmp = sgx_ocalloc(ocalloc_size);
	if (__tmp == NULL) {
		sgx_ocfree();
		return SGX_ERROR_UNEXPECTED;
	}
	ms = (ms_ocall_sgx_recv_t*)__tmp;
	__tmp = (void *)((size_t)__tmp + sizeof(ms_ocall_sgx_recv_t));
	ocalloc_size -= sizeof(ms_ocall_sgx_recv_t);

	ms->ms_s = s;
	if (buf != NULL) {
		ms->ms_buf = (void*)__tmp;
		__tmp_buf = __tmp;
		memset(__tmp_buf, 0, _len_buf);
		__tmp = (void *)((size_t)__tmp + _len_buf);
		ocalloc_size -= _len_buf;
	} else {
		ms->ms_buf = NULL;
	}
	
	ms->ms_len = len;
	ms->ms_flags = flags;
	status = sgx_ocall(22, ms);

	if (status == SGX_SUCCESS) {
		if (retval) *retval = ms->ms_retval;
		if (buf) {
			if (memcpy_s((void*)buf, _len_buf, __tmp_buf, _len_buf)) {
				sgx_ocfree();
				return SGX_ERROR_UNEXPECTED;
			}
		}
	}
	sgx_ocfree();
	return status;
}

sgx_status_t SGX_CDECL ocall_sgx_sendto(ssize_t* retval, int sockfd, const void* buf, size_t len, int flags, const struct sockaddr* dest_addr, size_t addrlen)
{
	sgx_status_t status = SGX_SUCCESS;
	size_t _len_buf = len;
	size_t _len_dest_addr = addrlen;

	ms_ocall_sgx_sendto_t* ms = NULL;
	size_t ocalloc_size = sizeof(ms_ocall_sgx_sendto_t);
	void *__tmp = NULL;


	CHECK_ENCLAVE_POINTER(buf, _len_buf);
	CHECK_ENCLAVE_POINTER(dest_addr, _len_dest_addr);

	if (ADD_ASSIGN_OVERFLOW(ocalloc_size, (buf != NULL) ? _len_buf : 0))
		return SGX_ERROR_INVALID_PARAMETER;
	if (ADD_ASSIGN_OVERFLOW(ocalloc_size, (dest_addr != NULL) ? _len_dest_addr : 0))
		return SGX_ERROR_INVALID_PARAMETER;

	__tmp = sgx_ocalloc(ocalloc_size);
	if (__tmp == NULL) {
		sgx_ocfree();
		return SGX_ERROR_UNEXPECTED;
	}
	ms = (ms_ocall_sgx_sendto_t*)__tmp;
	__tmp = (void *)((size_t)__tmp + sizeof(ms_ocall_sgx_sendto_t));
	ocalloc_size -= sizeof(ms_ocall_sgx_sendto_t);

	ms->ms_sockfd = sockfd;
	if (buf != NULL) {
		ms->ms_buf = (const void*)__tmp;
		if (memcpy_s(__tmp, ocalloc_size, buf, _len_buf)) {
			sgx_ocfree();
			return SGX_ERROR_UNEXPECTED;
		}
		__tmp = (void *)((size_t)__tmp + _len_buf);
		ocalloc_size -= _len_buf;
	} else {
		ms->ms_buf = NULL;
	}
	
	ms->ms_len = len;
	ms->ms_flags = flags;
	if (dest_addr != NULL) {
		ms->ms_dest_addr = (const struct sockaddr*)__tmp;
		if (memcpy_s(__tmp, ocalloc_size, dest_addr, _len_dest_addr)) {
			sgx_ocfree();
			return SGX_ERROR_UNEXPECTED;
		}
		__tmp = (void *)((size_t)__tmp + _len_dest_addr);
		ocalloc_size -= _len_dest_addr;
	} else {
		ms->ms_dest_addr = NULL;
	}
	
	ms->ms_addrlen = addrlen;
	status = sgx_ocall(23, ms);

	if (status == SGX_SUCCESS) {
		if (retval) *retval = ms->ms_retval;
	}
	sgx_ocfree();
	return status;
}

sgx_status_t SGX_CDECL ocall_sgx_recvfrom(ssize_t* retval, int s, void* buf, size_t len, int flags, struct sockaddr* dest_addr, unsigned int alen, socklen_t* addrlen)
{
	sgx_status_t status = SGX_SUCCESS;
	size_t _len_buf = len;
	size_t _len_dest_addr = alen;
	size_t _len_addrlen = sizeof(socklen_t);

	ms_ocall_sgx_recvfrom_t* ms = NULL;
	size_t ocalloc_size = sizeof(ms_ocall_sgx_recvfrom_t);
	void *__tmp = NULL;

	void *__tmp_buf = NULL;
	void *__tmp_dest_addr = NULL;
	void *__tmp_addrlen = NULL;

	CHECK_ENCLAVE_POINTER(buf, _len_buf);
	CHECK_ENCLAVE_POINTER(dest_addr, _len_dest_addr);
	CHECK_ENCLAVE_POINTER(addrlen, _len_addrlen);

	if (ADD_ASSIGN_OVERFLOW(ocalloc_size, (buf != NULL) ? _len_buf : 0))
		return SGX_ERROR_INVALID_PARAMETER;
	if (ADD_ASSIGN_OVERFLOW(ocalloc_size, (dest_addr != NULL) ? _len_dest_addr : 0))
		return SGX_ERROR_INVALID_PARAMETER;
	if (ADD_ASSIGN_OVERFLOW(ocalloc_size, (addrlen != NULL) ? _len_addrlen : 0))
		return SGX_ERROR_INVALID_PARAMETER;

	__tmp = sgx_ocalloc(ocalloc_size);
	if (__tmp == NULL) {
		sgx_ocfree();
		return SGX_ERROR_UNEXPECTED;
	}
	ms = (ms_ocall_sgx_recvfrom_t*)__tmp;
	__tmp = (void *)((size_t)__tmp + sizeof(ms_ocall_sgx_recvfrom_t));
	ocalloc_size -= sizeof(ms_ocall_sgx_recvfrom_t);

	ms->ms_s = s;
	if (buf != NULL) {
		ms->ms_buf = (void*)__tmp;
		__tmp_buf = __tmp;
		memset(__tmp_buf, 0, _len_buf);
		__tmp = (void *)((size_t)__tmp + _len_buf);
		ocalloc_size -= _len_buf;
	} else {
		ms->ms_buf = NULL;
	}
	
	ms->ms_len = len;
	ms->ms_flags = flags;
	if (dest_addr != NULL) {
		ms->ms_dest_addr = (struct sockaddr*)__tmp;
		__tmp_dest_addr = __tmp;
		memset(__tmp_dest_addr, 0, _len_dest_addr);
		__tmp = (void *)((size_t)__tmp + _len_dest_addr);
		ocalloc_size -= _len_dest_addr;
	} else {
		ms->ms_dest_addr = NULL;
	}
	
	ms->ms_alen = alen;
	if (addrlen != NULL) {
		ms->ms_addrlen = (socklen_t*)__tmp;
		__tmp_addrlen = __tmp;
		memset(__tmp_addrlen, 0, _len_addrlen);
		__tmp = (void *)((size_t)__tmp + _len_addrlen);
		ocalloc_size -= _len_addrlen;
	} else {
		ms->ms_addrlen = NULL;
	}
	
	status = sgx_ocall(24, ms);

	if (status == SGX_SUCCESS) {
		if (retval) *retval = ms->ms_retval;
		if (buf) {
			if (memcpy_s((void*)buf, _len_buf, __tmp_buf, _len_buf)) {
				sgx_ocfree();
				return SGX_ERROR_UNEXPECTED;
			}
		}
		if (dest_addr) {
			if (memcpy_s((void*)dest_addr, _len_dest_addr, __tmp_dest_addr, _len_dest_addr)) {
				sgx_ocfree();
				return SGX_ERROR_UNEXPECTED;
			}
		}
		if (addrlen) {
			if (memcpy_s((void*)addrlen, _len_addrlen, __tmp_addrlen, _len_addrlen)) {
				sgx_ocfree();
				return SGX_ERROR_UNEXPECTED;
			}
		}
	}
	sgx_ocfree();
	return status;
}

sgx_status_t SGX_CDECL ocall_sgx_gethostname(int* retval, char* name, size_t namelen)
{
	sgx_status_t status = SGX_SUCCESS;
	size_t _len_name = namelen;

	ms_ocall_sgx_gethostname_t* ms = NULL;
	size_t ocalloc_size = sizeof(ms_ocall_sgx_gethostname_t);
	void *__tmp = NULL;

	void *__tmp_name = NULL;

	CHECK_ENCLAVE_POINTER(name, _len_name);

	if (ADD_ASSIGN_OVERFLOW(ocalloc_size, (name != NULL) ? _len_name : 0))
		return SGX_ERROR_INVALID_PARAMETER;

	__tmp = sgx_ocalloc(ocalloc_size);
	if (__tmp == NULL) {
		sgx_ocfree();
		return SGX_ERROR_UNEXPECTED;
	}
	ms = (ms_ocall_sgx_gethostname_t*)__tmp;
	__tmp = (void *)((size_t)__tmp + sizeof(ms_ocall_sgx_gethostname_t));
	ocalloc_size -= sizeof(ms_ocall_sgx_gethostname_t);

	if (name != NULL) {
		ms->ms_name = (char*)__tmp;
		__tmp_name = __tmp;
		if (_len_name % sizeof(*name) != 0) {
			sgx_ocfree();
			return SGX_ERROR_INVALID_PARAMETER;
		}
		memset(__tmp_name, 0, _len_name);
		__tmp = (void *)((size_t)__tmp + _len_name);
		ocalloc_size -= _len_name;
	} else {
		ms->ms_name = NULL;
	}
	
	ms->ms_namelen = namelen;
	status = sgx_ocall(25, ms);

	if (status == SGX_SUCCESS) {
		if (retval) *retval = ms->ms_retval;
		if (name) {
			if (memcpy_s((void*)name, _len_name, __tmp_name, _len_name)) {
				sgx_ocfree();
				return SGX_ERROR_UNEXPECTED;
			}
		}
	}
	sgx_ocfree();
	return status;
}

sgx_status_t SGX_CDECL ocall_sgx_getaddrinfo(int* retval, const char* node, const char* service, const struct addrinfo* hints, unsigned long int* res)
{
	sgx_status_t status = SGX_SUCCESS;
	size_t _len_node = node ? strlen(node) + 1 : 0;
	size_t _len_service = service ? strlen(service) + 1 : 0;
	size_t _len_hints = 48;
	size_t _len_res = sizeof(unsigned long int);

	ms_ocall_sgx_getaddrinfo_t* ms = NULL;
	size_t ocalloc_size = sizeof(ms_ocall_sgx_getaddrinfo_t);
	void *__tmp = NULL;

	void *__tmp_res = NULL;

	CHECK_ENCLAVE_POINTER(node, _len_node);
	CHECK_ENCLAVE_POINTER(service, _len_service);
	CHECK_ENCLAVE_POINTER(hints, _len_hints);
	CHECK_ENCLAVE_POINTER(res, _len_res);

	if (ADD_ASSIGN_OVERFLOW(ocalloc_size, (node != NULL) ? _len_node : 0))
		return SGX_ERROR_INVALID_PARAMETER;
	if (ADD_ASSIGN_OVERFLOW(ocalloc_size, (service != NULL) ? _len_service : 0))
		return SGX_ERROR_INVALID_PARAMETER;
	if (ADD_ASSIGN_OVERFLOW(ocalloc_size, (hints != NULL) ? _len_hints : 0))
		return SGX_ERROR_INVALID_PARAMETER;
	if (ADD_ASSIGN_OVERFLOW(ocalloc_size, (res != NULL) ? _len_res : 0))
		return SGX_ERROR_INVALID_PARAMETER;

	__tmp = sgx_ocalloc(ocalloc_size);
	if (__tmp == NULL) {
		sgx_ocfree();
		return SGX_ERROR_UNEXPECTED;
	}
	ms = (ms_ocall_sgx_getaddrinfo_t*)__tmp;
	__tmp = (void *)((size_t)__tmp + sizeof(ms_ocall_sgx_getaddrinfo_t));
	ocalloc_size -= sizeof(ms_ocall_sgx_getaddrinfo_t);

	if (node != NULL) {
		ms->ms_node = (const char*)__tmp;
		if (_len_node % sizeof(*node) != 0) {
			sgx_ocfree();
			return SGX_ERROR_INVALID_PARAMETER;
		}
		if (memcpy_s(__tmp, ocalloc_size, node, _len_node)) {
			sgx_ocfree();
			return SGX_ERROR_UNEXPECTED;
		}
		__tmp = (void *)((size_t)__tmp + _len_node);
		ocalloc_size -= _len_node;
	} else {
		ms->ms_node = NULL;
	}
	
	if (service != NULL) {
		ms->ms_service = (const char*)__tmp;
		if (_len_service % sizeof(*service) != 0) {
			sgx_ocfree();
			return SGX_ERROR_INVALID_PARAMETER;
		}
		if (memcpy_s(__tmp, ocalloc_size, service, _len_service)) {
			sgx_ocfree();
			return SGX_ERROR_UNEXPECTED;
		}
		__tmp = (void *)((size_t)__tmp + _len_service);
		ocalloc_size -= _len_service;
	} else {
		ms->ms_service = NULL;
	}
	
	if (hints != NULL) {
		ms->ms_hints = (const struct addrinfo*)__tmp;
		if (memcpy_s(__tmp, ocalloc_size, hints, _len_hints)) {
			sgx_ocfree();
			return SGX_ERROR_UNEXPECTED;
		}
		__tmp = (void *)((size_t)__tmp + _len_hints);
		ocalloc_size -= _len_hints;
	} else {
		ms->ms_hints = NULL;
	}
	
	if (res != NULL) {
		ms->ms_res = (unsigned long int*)__tmp;
		__tmp_res = __tmp;
		if (_len_res % sizeof(*res) != 0) {
			sgx_ocfree();
			return SGX_ERROR_INVALID_PARAMETER;
		}
		memset(__tmp_res, 0, _len_res);
		__tmp = (void *)((size_t)__tmp + _len_res);
		ocalloc_size -= _len_res;
	} else {
		ms->ms_res = NULL;
	}
	
	status = sgx_ocall(26, ms);

	if (status == SGX_SUCCESS) {
		if (retval) *retval = ms->ms_retval;
		if (res) {
			if (memcpy_s((void*)res, _len_res, __tmp_res, _len_res)) {
				sgx_ocfree();
				return SGX_ERROR_UNEXPECTED;
			}
		}
	}
	sgx_ocfree();
	return status;
}

sgx_status_t SGX_CDECL ocall_sgx_getenv(char** retval, const char* env)
{
	sgx_status_t status = SGX_SUCCESS;
	size_t _len_env = env ? strlen(env) + 1 : 0;

	ms_ocall_sgx_getenv_t* ms = NULL;
	size_t ocalloc_size = sizeof(ms_ocall_sgx_getenv_t);
	void *__tmp = NULL;


	CHECK_ENCLAVE_POINTER(env, _len_env);

	if (ADD_ASSIGN_OVERFLOW(ocalloc_size, (env != NULL) ? _len_env : 0))
		return SGX_ERROR_INVALID_PARAMETER;

	__tmp = sgx_ocalloc(ocalloc_size);
	if (__tmp == NULL) {
		sgx_ocfree();
		return SGX_ERROR_UNEXPECTED;
	}
	ms = (ms_ocall_sgx_getenv_t*)__tmp;
	__tmp = (void *)((size_t)__tmp + sizeof(ms_ocall_sgx_getenv_t));
	ocalloc_size -= sizeof(ms_ocall_sgx_getenv_t);

	if (env != NULL) {
		ms->ms_env = (const char*)__tmp;
		if (_len_env % sizeof(*env) != 0) {
			sgx_ocfree();
			return SGX_ERROR_INVALID_PARAMETER;
		}
		if (memcpy_s(__tmp, ocalloc_size, env, _len_env)) {
			sgx_ocfree();
			return SGX_ERROR_UNEXPECTED;
		}
		__tmp = (void *)((size_t)__tmp + _len_env);
		ocalloc_size -= _len_env;
	} else {
		ms->ms_env = NULL;
	}
	
	status = sgx_ocall(27, ms);

	if (status == SGX_SUCCESS) {
		if (retval) *retval = ms->ms_retval;
	}
	sgx_ocfree();
	return status;
}

sgx_status_t SGX_CDECL ocall_sgx_getsockname(int* retval, int s, struct sockaddr* name, unsigned int nlen, socklen_t* addrlen)
{
	sgx_status_t status = SGX_SUCCESS;
	size_t _len_name = nlen;
	size_t _len_addrlen = sizeof(socklen_t);

	ms_ocall_sgx_getsockname_t* ms = NULL;
	size_t ocalloc_size = sizeof(ms_ocall_sgx_getsockname_t);
	void *__tmp = NULL;

	void *__tmp_name = NULL;
	void *__tmp_addrlen = NULL;

	CHECK_ENCLAVE_POINTER(name, _len_name);
	CHECK_ENCLAVE_POINTER(addrlen, _len_addrlen);

	if (ADD_ASSIGN_OVERFLOW(ocalloc_size, (name != NULL) ? _len_name : 0))
		return SGX_ERROR_INVALID_PARAMETER;
	if (ADD_ASSIGN_OVERFLOW(ocalloc_size, (addrlen != NULL) ? _len_addrlen : 0))
		return SGX_ERROR_INVALID_PARAMETER;

	__tmp = sgx_ocalloc(ocalloc_size);
	if (__tmp == NULL) {
		sgx_ocfree();
		return SGX_ERROR_UNEXPECTED;
	}
	ms = (ms_ocall_sgx_getsockname_t*)__tmp;
	__tmp = (void *)((size_t)__tmp + sizeof(ms_ocall_sgx_getsockname_t));
	ocalloc_size -= sizeof(ms_ocall_sgx_getsockname_t);

	ms->ms_s = s;
	if (name != NULL) {
		ms->ms_name = (struct sockaddr*)__tmp;
		__tmp_name = __tmp;
		memset(__tmp_name, 0, _len_name);
		__tmp = (void *)((size_t)__tmp + _len_name);
		ocalloc_size -= _len_name;
	} else {
		ms->ms_name = NULL;
	}
	
	ms->ms_nlen = nlen;
	if (addrlen != NULL) {
		ms->ms_addrlen = (socklen_t*)__tmp;
		__tmp_addrlen = __tmp;
		memset(__tmp_addrlen, 0, _len_addrlen);
		__tmp = (void *)((size_t)__tmp + _len_addrlen);
		ocalloc_size -= _len_addrlen;
	} else {
		ms->ms_addrlen = NULL;
	}
	
	status = sgx_ocall(28, ms);

	if (status == SGX_SUCCESS) {
		if (retval) *retval = ms->ms_retval;
		if (name) {
			if (memcpy_s((void*)name, _len_name, __tmp_name, _len_name)) {
				sgx_ocfree();
				return SGX_ERROR_UNEXPECTED;
			}
		}
		if (addrlen) {
			if (memcpy_s((void*)addrlen, _len_addrlen, __tmp_addrlen, _len_addrlen)) {
				sgx_ocfree();
				return SGX_ERROR_UNEXPECTED;
			}
		}
	}
	sgx_ocfree();
	return status;
}

sgx_status_t SGX_CDECL ocall_sgx_getsockopt(int* retval, int s, int level, int optname, void* optval, unsigned int len, socklen_t* optlen)
{
	sgx_status_t status = SGX_SUCCESS;
	size_t _len_optval = len;
	size_t _len_optlen = sizeof(socklen_t);

	ms_ocall_sgx_getsockopt_t* ms = NULL;
	size_t ocalloc_size = sizeof(ms_ocall_sgx_getsockopt_t);
	void *__tmp = NULL;

	void *__tmp_optval = NULL;
	void *__tmp_optlen = NULL;

	CHECK_ENCLAVE_POINTER(optval, _len_optval);
	CHECK_ENCLAVE_POINTER(optlen, _len_optlen);

	if (ADD_ASSIGN_OVERFLOW(ocalloc_size, (optval != NULL) ? _len_optval : 0))
		return SGX_ERROR_INVALID_PARAMETER;
	if (ADD_ASSIGN_OVERFLOW(ocalloc_size, (optlen != NULL) ? _len_optlen : 0))
		return SGX_ERROR_INVALID_PARAMETER;

	__tmp = sgx_ocalloc(ocalloc_size);
	if (__tmp == NULL) {
		sgx_ocfree();
		return SGX_ERROR_UNEXPECTED;
	}
	ms = (ms_ocall_sgx_getsockopt_t*)__tmp;
	__tmp = (void *)((size_t)__tmp + sizeof(ms_ocall_sgx_getsockopt_t));
	ocalloc_size -= sizeof(ms_ocall_sgx_getsockopt_t);

	ms->ms_s = s;
	ms->ms_level = level;
	ms->ms_optname = optname;
	if (optval != NULL) {
		ms->ms_optval = (void*)__tmp;
		__tmp_optval = __tmp;
		memset(__tmp_optval, 0, _len_optval);
		__tmp = (void *)((size_t)__tmp + _len_optval);
		ocalloc_size -= _len_optval;
	} else {
		ms->ms_optval = NULL;
	}
	
	ms->ms_len = len;
	if (optlen != NULL) {
		ms->ms_optlen = (socklen_t*)__tmp;
		__tmp_optlen = __tmp;
		memset(__tmp_optlen, 0, _len_optlen);
		__tmp = (void *)((size_t)__tmp + _len_optlen);
		ocalloc_size -= _len_optlen;
	} else {
		ms->ms_optlen = NULL;
	}
	
	status = sgx_ocall(29, ms);

	if (status == SGX_SUCCESS) {
		if (retval) *retval = ms->ms_retval;
		if (optval) {
			if (memcpy_s((void*)optval, _len_optval, __tmp_optval, _len_optval)) {
				sgx_ocfree();
				return SGX_ERROR_UNEXPECTED;
			}
		}
		if (optlen) {
			if (memcpy_s((void*)optlen, _len_optlen, __tmp_optlen, _len_optlen)) {
				sgx_ocfree();
				return SGX_ERROR_UNEXPECTED;
			}
		}
	}
	sgx_ocfree();
	return status;
}

sgx_status_t SGX_CDECL ocall_sgx_getservbyname(struct servent** retval, const char* name, const char* proto)
{
	sgx_status_t status = SGX_SUCCESS;
	size_t _len_name = name ? strlen(name) + 1 : 0;
	size_t _len_proto = proto ? strlen(proto) + 1 : 0;

	ms_ocall_sgx_getservbyname_t* ms = NULL;
	size_t ocalloc_size = sizeof(ms_ocall_sgx_getservbyname_t);
	void *__tmp = NULL;


	CHECK_ENCLAVE_POINTER(name, _len_name);
	CHECK_ENCLAVE_POINTER(proto, _len_proto);

	if (ADD_ASSIGN_OVERFLOW(ocalloc_size, (name != NULL) ? _len_name : 0))
		return SGX_ERROR_INVALID_PARAMETER;
	if (ADD_ASSIGN_OVERFLOW(ocalloc_size, (proto != NULL) ? _len_proto : 0))
		return SGX_ERROR_INVALID_PARAMETER;

	__tmp = sgx_ocalloc(ocalloc_size);
	if (__tmp == NULL) {
		sgx_ocfree();
		return SGX_ERROR_UNEXPECTED;
	}
	ms = (ms_ocall_sgx_getservbyname_t*)__tmp;
	__tmp = (void *)((size_t)__tmp + sizeof(ms_ocall_sgx_getservbyname_t));
	ocalloc_size -= sizeof(ms_ocall_sgx_getservbyname_t);

	if (name != NULL) {
		ms->ms_name = (const char*)__tmp;
		if (_len_name % sizeof(*name) != 0) {
			sgx_ocfree();
			return SGX_ERROR_INVALID_PARAMETER;
		}
		if (memcpy_s(__tmp, ocalloc_size, name, _len_name)) {
			sgx_ocfree();
			return SGX_ERROR_UNEXPECTED;
		}
		__tmp = (void *)((size_t)__tmp + _len_name);
		ocalloc_size -= _len_name;
	} else {
		ms->ms_name = NULL;
	}
	
	if (proto != NULL) {
		ms->ms_proto = (const char*)__tmp;
		if (_len_proto % sizeof(*proto) != 0) {
			sgx_ocfree();
			return SGX_ERROR_INVALID_PARAMETER;
		}
		if (memcpy_s(__tmp, ocalloc_size, proto, _len_proto)) {
			sgx_ocfree();
			return SGX_ERROR_UNEXPECTED;
		}
		__tmp = (void *)((size_t)__tmp + _len_proto);
		ocalloc_size -= _len_proto;
	} else {
		ms->ms_proto = NULL;
	}
	
	status = sgx_ocall(30, ms);

	if (status == SGX_SUCCESS) {
		if (retval) *retval = ms->ms_retval;
	}
	sgx_ocfree();
	return status;
}

sgx_status_t SGX_CDECL ocall_sgx_getprotobynumber(struct protoent** retval, int proto)
{
	sgx_status_t status = SGX_SUCCESS;

	ms_ocall_sgx_getprotobynumber_t* ms = NULL;
	size_t ocalloc_size = sizeof(ms_ocall_sgx_getprotobynumber_t);
	void *__tmp = NULL;


	__tmp = sgx_ocalloc(ocalloc_size);
	if (__tmp == NULL) {
		sgx_ocfree();
		return SGX_ERROR_UNEXPECTED;
	}
	ms = (ms_ocall_sgx_getprotobynumber_t*)__tmp;
	__tmp = (void *)((size_t)__tmp + sizeof(ms_ocall_sgx_getprotobynumber_t));
	ocalloc_size -= sizeof(ms_ocall_sgx_getprotobynumber_t);

	ms->ms_proto = proto;
	status = sgx_ocall(31, ms);

	if (status == SGX_SUCCESS) {
		if (retval) *retval = ms->ms_retval;
	}
	sgx_ocfree();
	return status;
}

sgx_status_t SGX_CDECL ocall_sgx_setsockopt(int* retval, int s, int level, int optname, const void* optval, size_t optlen)
{
	sgx_status_t status = SGX_SUCCESS;
	size_t _len_optval = optlen;

	ms_ocall_sgx_setsockopt_t* ms = NULL;
	size_t ocalloc_size = sizeof(ms_ocall_sgx_setsockopt_t);
	void *__tmp = NULL;


	CHECK_ENCLAVE_POINTER(optval, _len_optval);

	if (ADD_ASSIGN_OVERFLOW(ocalloc_size, (optval != NULL) ? _len_optval : 0))
		return SGX_ERROR_INVALID_PARAMETER;

	__tmp = sgx_ocalloc(ocalloc_size);
	if (__tmp == NULL) {
		sgx_ocfree();
		return SGX_ERROR_UNEXPECTED;
	}
	ms = (ms_ocall_sgx_setsockopt_t*)__tmp;
	__tmp = (void *)((size_t)__tmp + sizeof(ms_ocall_sgx_setsockopt_t));
	ocalloc_size -= sizeof(ms_ocall_sgx_setsockopt_t);

	ms->ms_s = s;
	ms->ms_level = level;
	ms->ms_optname = optname;
	if (optval != NULL) {
		ms->ms_optval = (const void*)__tmp;
		if (memcpy_s(__tmp, ocalloc_size, optval, _len_optval)) {
			sgx_ocfree();
			return SGX_ERROR_UNEXPECTED;
		}
		__tmp = (void *)((size_t)__tmp + _len_optval);
		ocalloc_size -= _len_optval;
	} else {
		ms->ms_optval = NULL;
	}
	
	ms->ms_optlen = optlen;
	status = sgx_ocall(32, ms);

	if (status == SGX_SUCCESS) {
		if (retval) *retval = ms->ms_retval;
	}
	sgx_ocfree();
	return status;
}

sgx_status_t SGX_CDECL ocall_sgx_htons(unsigned short int* retval, unsigned short int hostshort)
{
	sgx_status_t status = SGX_SUCCESS;

	ms_ocall_sgx_htons_t* ms = NULL;
	size_t ocalloc_size = sizeof(ms_ocall_sgx_htons_t);
	void *__tmp = NULL;


	__tmp = sgx_ocalloc(ocalloc_size);
	if (__tmp == NULL) {
		sgx_ocfree();
		return SGX_ERROR_UNEXPECTED;
	}
	ms = (ms_ocall_sgx_htons_t*)__tmp;
	__tmp = (void *)((size_t)__tmp + sizeof(ms_ocall_sgx_htons_t));
	ocalloc_size -= sizeof(ms_ocall_sgx_htons_t);

	ms->ms_hostshort = hostshort;
	status = sgx_ocall(33, ms);

	if (status == SGX_SUCCESS) {
		if (retval) *retval = ms->ms_retval;
	}
	sgx_ocfree();
	return status;
}

sgx_status_t SGX_CDECL ocall_sgx_htonl(unsigned long int* retval, unsigned long int hostlong)
{
	sgx_status_t status = SGX_SUCCESS;

	ms_ocall_sgx_htonl_t* ms = NULL;
	size_t ocalloc_size = sizeof(ms_ocall_sgx_htonl_t);
	void *__tmp = NULL;


	__tmp = sgx_ocalloc(ocalloc_size);
	if (__tmp == NULL) {
		sgx_ocfree();
		return SGX_ERROR_UNEXPECTED;
	}
	ms = (ms_ocall_sgx_htonl_t*)__tmp;
	__tmp = (void *)((size_t)__tmp + sizeof(ms_ocall_sgx_htonl_t));
	ocalloc_size -= sizeof(ms_ocall_sgx_htonl_t);

	ms->ms_hostlong = hostlong;
	status = sgx_ocall(34, ms);

	if (status == SGX_SUCCESS) {
		if (retval) *retval = ms->ms_retval;
	}
	sgx_ocfree();
	return status;
}

sgx_status_t SGX_CDECL ocall_sgx_ntohs(unsigned short int* retval, unsigned short int netshort)
{
	sgx_status_t status = SGX_SUCCESS;

	ms_ocall_sgx_ntohs_t* ms = NULL;
	size_t ocalloc_size = sizeof(ms_ocall_sgx_ntohs_t);
	void *__tmp = NULL;


	__tmp = sgx_ocalloc(ocalloc_size);
	if (__tmp == NULL) {
		sgx_ocfree();
		return SGX_ERROR_UNEXPECTED;
	}
	ms = (ms_ocall_sgx_ntohs_t*)__tmp;
	__tmp = (void *)((size_t)__tmp + sizeof(ms_ocall_sgx_ntohs_t));
	ocalloc_size -= sizeof(ms_ocall_sgx_ntohs_t);

	ms->ms_netshort = netshort;
	status = sgx_ocall(35, ms);

	if (status == SGX_SUCCESS) {
		if (retval) *retval = ms->ms_retval;
	}
	sgx_ocfree();
	return status;
}

sgx_status_t SGX_CDECL ocall_sgx_ntohl(unsigned long int* retval, unsigned long int netlong)
{
	sgx_status_t status = SGX_SUCCESS;

	ms_ocall_sgx_ntohl_t* ms = NULL;
	size_t ocalloc_size = sizeof(ms_ocall_sgx_ntohl_t);
	void *__tmp = NULL;


	__tmp = sgx_ocalloc(ocalloc_size);
	if (__tmp == NULL) {
		sgx_ocfree();
		return SGX_ERROR_UNEXPECTED;
	}
	ms = (ms_ocall_sgx_ntohl_t*)__tmp;
	__tmp = (void *)((size_t)__tmp + sizeof(ms_ocall_sgx_ntohl_t));
	ocalloc_size -= sizeof(ms_ocall_sgx_ntohl_t);

	ms->ms_netlong = netlong;
	status = sgx_ocall(36, ms);

	if (status == SGX_SUCCESS) {
		if (retval) *retval = ms->ms_retval;
	}
	sgx_ocfree();
	return status;
}

sgx_status_t SGX_CDECL ocall_sgx_signal(sighandler_t* retval, int signum, sighandler_t a)
{
	sgx_status_t status = SGX_SUCCESS;

	ms_ocall_sgx_signal_t* ms = NULL;
	size_t ocalloc_size = sizeof(ms_ocall_sgx_signal_t);
	void *__tmp = NULL;


	__tmp = sgx_ocalloc(ocalloc_size);
	if (__tmp == NULL) {
		sgx_ocfree();
		return SGX_ERROR_UNEXPECTED;
	}
	ms = (ms_ocall_sgx_signal_t*)__tmp;
	__tmp = (void *)((size_t)__tmp + sizeof(ms_ocall_sgx_signal_t));
	ocalloc_size -= sizeof(ms_ocall_sgx_signal_t);

	ms->ms_signum = signum;
	ms->ms_a = a;
	status = sgx_ocall(37, ms);

	if (status == SGX_SUCCESS) {
		if (retval) *retval = ms->ms_retval;
	}
	sgx_ocfree();
	return status;
}

sgx_status_t SGX_CDECL ocall_sgx_shutdown(int* retval, int a, int b)
{
	sgx_status_t status = SGX_SUCCESS;

	ms_ocall_sgx_shutdown_t* ms = NULL;
	size_t ocalloc_size = sizeof(ms_ocall_sgx_shutdown_t);
	void *__tmp = NULL;


	__tmp = sgx_ocalloc(ocalloc_size);
	if (__tmp == NULL) {
		sgx_ocfree();
		return SGX_ERROR_UNEXPECTED;
	}
	ms = (ms_ocall_sgx_shutdown_t*)__tmp;
	__tmp = (void *)((size_t)__tmp + sizeof(ms_ocall_sgx_shutdown_t));
	ocalloc_size -= sizeof(ms_ocall_sgx_shutdown_t);

	ms->ms_a = a;
	ms->ms_b = b;
	status = sgx_ocall(38, ms);

	if (status == SGX_SUCCESS) {
		if (retval) *retval = ms->ms_retval;
	}
	sgx_ocfree();
	return status;
}

