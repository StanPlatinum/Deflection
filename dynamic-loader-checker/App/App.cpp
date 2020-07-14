/*
 * Copyright (C) 2011-2016 Intel Corporation. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in
 *     the documentation and/or other materials provided with the
 *     distribution.
 *   * Neither the name of Intel Corporation nor the names of its
 *     contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */


#include <stdio.h>
#include <string.h>
#include <assert.h>

# include <pwd.h>

/* man 2 open */
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

/* man 2 stat, read */
#include <unistd.h>

/* man 2 mmap */
#include <sys/mman.h>

typedef void (*sighandler_t)(int);

#include "sgx_urts.h"
#include "App.h"
#include "Enclave_u.h"

typedef struct _sgx_errlist_t {
	sgx_status_t err;
	const char *msg;
	const char *sug; /* Suggestion */
} sgx_errlist_t;

/* Error code returned by sgx_create_enclave */
static sgx_errlist_t sgx_errlist[] = {
	{
		SGX_ERROR_UNEXPECTED,
		"Unexpected error occurred.",
		NULL
	},
	{
		SGX_ERROR_INVALID_PARAMETER,
		"Invalid parameter.",
		NULL
	},
	{
		SGX_ERROR_OUT_OF_MEMORY,
		"Out of memory.",
		NULL
	},
	{
		SGX_ERROR_ENCLAVE_LOST,
		"Power transition occurred.",
		"Please refer to the sample \"PowerTransition\" for details."
	},
	{
		SGX_ERROR_INVALID_ENCLAVE,
		"Invalid enclave image.",
		NULL
	},
	{
		SGX_ERROR_INVALID_ENCLAVE_ID,
		"Invalid enclave identification.",
		NULL
	},
	{
		SGX_ERROR_INVALID_SIGNATURE,
		"Invalid enclave signature.",
		NULL
	},
	{
		SGX_ERROR_OUT_OF_EPC,
		"Out of EPC memory.",
		NULL
	},
	{
		SGX_ERROR_NO_DEVICE,
		"Invalid SGX device.",
		"Please make sure SGX module is enabled in the BIOS, and install SGX driver afterwards."
	},
	{
		SGX_ERROR_MEMORY_MAP_CONFLICT,
		"Memory map conflicted.",
		NULL
	},
	{
		SGX_ERROR_INVALID_METADATA,
		"Invalid enclave metadata.",
		NULL
	},
	{
		SGX_ERROR_DEVICE_BUSY,
		"SGX device was busy.",
		NULL
	},
	{
		SGX_ERROR_INVALID_VERSION,
		"Enclave version was invalid.",
		NULL
	},
	{
		SGX_ERROR_INVALID_ATTRIBUTE,
		"Enclave was not authorized.",
		NULL
	},
	{
		SGX_ERROR_ENCLAVE_FILE_ACCESS,
		"Can't open enclave file.",
		NULL
	},
};

/* Check error conditions for loading enclave */
void print_error_message(sgx_status_t ret)
{
	size_t idx = 0;
	size_t ttl = sizeof sgx_errlist/sizeof sgx_errlist[0];

	for (idx = 0; idx < ttl; idx++) {
		if(ret == sgx_errlist[idx].err) {
			if(NULL != sgx_errlist[idx].sug)
				printf("Info: %s\n", sgx_errlist[idx].sug);
			printf("Error: %s\n", sgx_errlist[idx].msg);
			break;
		}
	}

	if (idx == ttl)
		printf("Error: Unexpected error occurred.\n");
}

/* Ocall functions */
void ocall_print_string(const char *str)
{
	/* Proxy/Bridge will check the length and null-terminate
	 * the input string to prevent buffer overflow.
	 */
	printf("%s", str);
}

static unsigned long base;
using namespace std;
#include <vector>
vector<unsigned long> gg;
void ocall_sgx_push_gadget(unsigned long gadget)
{
	gg.push_back(gadget);
}
#include <time.h>
#define PAGE_SIZE 4096
#define UNIT_SIZE 32
unsigned long guess_in_page(unsigned long addr)
{
	addr = (addr & -PAGE_SIZE); // clear lower bits
	return addr + (((unsigned long)clock()) & ((PAGE_SIZE/UNIT_SIZE)-1)) * UNIT_SIZE;
}
#define SPACE_SIZE 0x2000000
unsigned long guess_in_space(unsigned long addr)
{
	if (addr < gg[1])
		return gg[0] + (((unsigned long)clock()) & ((SPACE_SIZE/UNIT_SIZE)-1)) * UNIT_SIZE;
	else
		return gg[1] + (((unsigned long)clock()) & ((SPACE_SIZE/UNIT_SIZE)-1)) * UNIT_SIZE;
}
unsigned long guess_base(unsigned long addr)
{
	unsigned long _base = 0x7f73c0000000;
	if (addr < gg[1])
		return (gg[0] - base + _base)
			+ (((unsigned long)clock()) & ((SPACE_SIZE/UNIT_SIZE)-1)) * UNIT_SIZE;
	else
		return (gg[1] - base + _base)
			+ (((unsigned long)clock()) & ((SPACE_SIZE/UNIT_SIZE)-1)) * UNIT_SIZE;
}

#if 0
// Called by EEXIT attack
char stack_for_attack[0x80000];
const char *attack_msg = "ROP_EEXIT success";
extern "C" void attacked();
extern "C" void print_attack_msg(const char **msg) {
	puts(*msg);
	exit(1);
}
#endif

#include "ocall.cpp"

#ifdef LD_DEBUG
#define log(s) puts("app: " s)
#else
#define log(s)
#endif

#include <cstdlib>

/* Application entry */
int main(int argc, char *argv[])
{
	sgx_launch_token_t token = {0};
	sgx_status_t ret = SGX_ERROR_UNEXPECTED;
	int updated = 0;
	sgx_enclave_id_t eid = 0;

	log("initialize the enclave");
	ret = sgx_create_enclave(ENCLAVE_FILENAME, SGX_DEBUG_FLAG, &token, &updated, &eid, NULL);
	if (ret != SGX_SUCCESS) {
		print_error_message(ret);
		return -1;
	}
	log("call enclave main");


	char *buffer_data;
	FILE *data_fp = fopen("./data", "rb");
	if (data_fp == NULL) {
		perror("[error] file open failed.\n");
		exit(1);
	}
	// get file size
	fseek(data_fp, 0L, SEEK_END);
	size_t sz_data = ftell(data_fp);
	rewind(data_fp);
	printf("code file size is %ld\n", sz_data);
	// read binary file into buffer
	//buffer = (char *)malloc((sz+10)*sizeof(char));
	buffer_data = (char *)malloc( sz_data * sizeof(char) );
	int data_n_read = fread(buffer_data, sizeof(char), sz_data, data_fp);
	printf("number of bytes read is %d\n", data_n_read);
	
	//W: receive data
	ecall_receive_data(eid, buffer_data, sz_data);

	fclose(data_fp);
	printf("receiving/executing binary completed.\n");



	char *entrylabel_buffer;
	FILE *entrylabel_fp = fopen("./entryLabel.txt", "rb");
	if (entrylabel_fp == NULL) {
		perror("[error] file open failed.\n");
		exit(1);
	}
	// get file size
	fseek(entrylabel_fp, 0L, SEEK_END);
	size_t entrylabel_sz = ftell(entrylabel_fp);
	rewind(entrylabel_fp);
	printf("entrylabel file size is %ld\n", entrylabel_sz);
	entrylabel_buffer = (char *)malloc( (entrylabel_sz) * sizeof(char) );
	int entrylabel_n_read = fread(entrylabel_buffer, sizeof(char), entrylabel_sz, entrylabel_fp);
	printf("number of bytes read in entryLabel: %d\n", entrylabel_n_read);

	//W: receive label list
	ecall_receive_entrylabel(eid, entrylabel_buffer, entrylabel_sz);
	
	fclose(entrylabel_fp);
	printf("receive entrylabel completed.\n");

	//base = *(unsigned long *)((unsigned long )sgx_create_enclave+0x212098);
	char *buffer_code;
	FILE *fp = fopen("./program", "rb");
	//W: the following line is for nbench
	//FILE *fp = fopen("./nben", "rb");
	if (fp == NULL) {
		perror("[error] file open failed.\n");
		exit(1);
	}
	// get file size
	fseek(fp, 0L, SEEK_END);
	size_t sz_code = ftell(fp);
	rewind(fp);
	printf("code file size is %ld\n", sz_code);
	// read binary file into buffer
	//buffer = (char *)malloc((sz+10)*sizeof(char));
	buffer_code = (char *)malloc( sz_code * sizeof(char) );
	int n_read = fread(buffer_code, sizeof(char), sz_code, fp);
	printf("number of bytes read is %d\n", n_read);
	
	//W: receive code
	ecall_receive_binary(eid, buffer_code, sz_code);
	fclose(fp);
	printf("receiving/executing binary completed.\n");

	//W: call test ecall
	//ecall_test_capstone_iter(eid);

	log("destroying the enclave");
	sgx_destroy_enclave(eid);
	return 0;
}
