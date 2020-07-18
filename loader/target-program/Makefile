CC = /home/dajiejie/llvm-mc/build/bin/clang -fPIC -fno-asynchronous-unwind-tables -fno-addrsig

#CXX = clang++

#TARGET_NAME = simple_malloc
#TARGET_NAME = simple_loop
#TARGET_NAME = malloc_and_sort
#TARGET_NAME = hardware
#TARGET_NAME = foo2
#TARGET_NAME = nw
#TARGET_NAME = fasta

#TARGET_NAME = bm_malloc_and_sort
#TARGET_NAME = bm_clock
#TARGET_NAME = bm_malloc_magic
#TARGET_NAME = bm_malloc_memalign_magic
#TARGET_NAME = bm_memcpy
#TARGET_NAME = bm_memchr
#TARGET_NAME = bm_sprintf
#TARGET_NAME = bm_sort_and_binsearch
TARGET_NAME = bm_hello
#TARGET_NAME = file_io_test

#TARGET_NAME = credit_score

SSL_SERVER = ssl_server

BASE_CFLAGS = -Wall -pedantic -Wno-unused-function -std=gnu11

NOLIBC_FLAG = -nodefaultlibs -nostdlib

MUSL_LIBC_DIR = ./musl-libc
MUSL_LIBC = $(MUSL_LIBC_DIR)/lib/libc.a
SGX_CFLAGS = $(BASE_CFLAGS) -I$(MUSL_LIBC_DIR)/include -fno-stack-protector -fvisibility=hidden

#W: add Makefile in lib/ for ocalls
OCALL_OBJS_DIR = lib/
OCALL_LIB_MK = lib/ocall.mk
include $(OCALL_LIB_MK)

#DEBUG_OBJ = lib/my_itoa.o

ASM_OBJS = $(DEBUG_OBJ) lib/stack.o lib/start.o lib/ocall.o lib/rop_gadget.o


LIBGCC = libgcc/ashldi3.o libgcc/ashrdi3.o libgcc/__divide_error.o	\
		 libgcc/__divdi3.o libgcc/__divti3.o libgcc/__lshrdi3.o		\
		 libgcc/multi3.o libgcc/__negti2.o libgcc/__udivdi3.o		\
		 libgcc/__udivmoddi4.o libgcc/__udivmodti4.o				\
		 libgcc/__udivti3.o libgcc/__umoddi3.o libgcc/__umodti3.o


all: $(MUSL_LIBC) $(TARGET_NAME)
	mv $(TARGET_NAME) ../program

#$(DEBUG_OBJ): lib/my_itoa.c
#	$(CC) -c $< -o $@

$(MUSL_LIBC):
	(cd musl-libc; ./configure)
	make -C musl-libc lib/libc.a

SSL_FLAGS = -D_FILE_OFFSET_BITS=64 -I./mbedtls/include -I./musl-libc/include

LIBSSL = mbedtls/libmbedtls.a mbedtls/libmbedcrypto.a mbedtls/libmbedx509.a

$(LIBSSL):
	$(MAKE) -C mbedtls all

$(SSL_SERVER): objs/libc_sslserver.a tools/linker malloc.o $(OCALL_OBJS) $(ASM_OBJS) $(LIBGCC)
	cp lib/*.o objs
	cp libgcc/*.o objs

https: $(MUSL_LIBC) $(SSL_SERVER) $(LIBSSL)
	cp mbedtls/*.o objs
	ls objs/*.o > list
	cat list | ./tools/linker https
	mv https ../program
	@echo "Build successfully!"


LIBNBENCH = nbench/nbench.a
nben: $(TARGET_NAME) $(LIBNBENCH)
	cp nbench/*.o objs
	ls objs/*.o > list
	cat list | ./tools/linker nben
	mv nben ../
	rm $(TARGET_NAME)

$(LIBNBENCH):
	$(MAKE) -C nbench nbench.a


$(TARGET_NAME): objs/libc_targetprogram.a tools/linker malloc.o $(OCALL_OBJS) $(ASM_OBJS) $(LIBGCC)
	cp lib/*.o objs
	cp libgcc/*.o objs
	ls objs/*.o > list
	cat list | ./tools/linker $(TARGET_NAME)
	@echo "Build successfully!"
#cp malloc.o objs/

lib/%.o: lib/%.s 
	$(CC) -c -o $@ $<

tools/linker: tools/linkage_editor.cpp
	$(CXX) -o $@ $^

objs/libc_sslserver.a: $(SSL_SERVER).o malloc.o $(MUSL_LIBC)
	cp $^ objs/
	$(MAKE) -C objs libc_sslserver.a

objs/libc_targetprogram.a: enclave_main.o malloc.o $(MUSL_LIBC)
	cp $^ objs/
	$(MAKE) -C objs libc_targetprogram.a

malloc.o: malloc/malloc.c malloc/dlmalloc.inc
	$(CC) -c $(SGX_CFLAGS) -o $@ $<

enclave_main.o: $(TARGET_NAME).c enclave.h
	$(CC) -S -I. -o $(TARGET_NAME).s $<
	$(CC) -c $(TARGET_NAME).s -o $@

#mbedtls_Flag = -lmbedtls -lmbedcrypto -lmbedx509

$(SSL_SERVER).o: $(SSL_SERVER).c enclave.h
	$(CC) -S $(SSL_FLAGS) -o $(SSL_SERVER).s $< 
	$(CC) -c $(SSL_SERVER).s -o $@

clean:
	rm -f *.s *.o *.a objs/*.o objs/*.lo objs/*.a list $(TARGET_NAME)

clean-all:
	rm -f *.o *.a objs/*.o objs/*.lo objs/*.a list $(TARGET_NAME) \
                tools/linker lib/*.o
	$(MAKE) -C nbench clean
	$(MAKE) -C mbedtls clean
	$(MAKE) -C musl-libc clean
