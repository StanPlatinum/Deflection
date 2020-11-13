LLVM_MC_PATH = /home/dajiejie/llvm-mc

LLVM_BIN_PATH = $(LLVM_PATH)/build/bin
CFIHello_So_PATH = $(LLVM_PATH)/build/lib/LLVMCFIHello.so

#CC = $(LLVM_PATH)/build/bin/clang -fPIC -fno-asynchronous-unwind-tables -fno-addrsig -mstackrealign -Xclang -load -Xclang $(CFIHello_So_PATH)
CC = $(LLVM_MC_PATH)/build/bin/clang -fPIC -fno-asynchronous-unwind-tables -fno-addrsig -mstackrealign

# Also see "include/mbedtls/config.h"

BASE_CFLAGS = -Wall -pedantic -Wno-unused-function -std=gnu11

CFLAGS = $(BASE_CFLAGS) -I../musl-libc/include \
		 -fno-stack-protector -fvisibility=hidden

LOCAL_CFLAGS = -Iinclude -I../ -D_FILE_OFFSET_BITS=64

OBJS_CRYPTO=	aes.o		aesni.o		arc4.o		\
		asn1parse.o	asn1write.o	base64.o	\
		bignum.o	blowfish.o	camellia.o	\
		ccm.o		cipher.o	cipher_wrap.o	\
		ctr_drbg.o	des.o		dhm.o		\
		ecdh.o		ecdsa.o		ecjpake.o	\
		ecp.o						\
		ecp_curves.o	entropy.o	entropy_poll.o	\
		error.o		gcm.o		havege.o	\
		hmac_drbg.o	md.o		md2.o		\
		md4.o		md5.o		md_wrap.o	\
		memory_buffer_alloc.o		oid.o		\
		padlock.o	pem.o		pk.o		\
		pk_wrap.o	pkcs12.o	pkcs5.o		\
		pkparse.o	pkwrite.o	platform.o	\
		ripemd160.o	rsa.o		sha1.o		\
		sha256.o	sha512.o	threading.o	\
		timing.o	version.o			\
		version_features.o		xtea.o

OBJS_X509=	certs.o		pkcs11.o	x509.o		\
		x509_create.o	x509_crl.o	x509_crt.o	\
		x509_csr.o	x509write_crt.o	x509write_csr.o

OBJS_TLS=	debug.o		net.o		ssl_cache.o	\
		ssl_ciphersuites.o		ssl_cli.o	\
		ssl_cookie.o	ssl_srv.o	ssl_ticket.o	\
		ssl_tls.o

.PHONY: all static clean

all: static
	cat *.txt > entryLabel4ssl
	sort entryLabel4ssl | uniq > entryLabel4ssl.txt
	rm entryLabel4ssl
	mv entryLabel4ssl.txt ..
	rm *.txt

#static: libmbedcrypto.a libmbedx509.a libmbedtls.a ssl_server.o
static: libmbedcrypto.a libmbedx509.a libmbedtls.a

# tls
libmbedtls.a: $(OBJS_TLS)
	echo "  AR    $@"
	$(AR) -rc $@ $(OBJS_TLS)
	echo "  RL    $@"
	$(AR) -s $@

# x509
libmbedx509.a: $(OBJS_X509)
	echo "  AR    $@"
	$(AR) -rc $@ $(OBJS_X509)
	echo "  RL    $@"
	$(AR) -s $@

# crypto
libmbedcrypto.a: $(OBJS_CRYPTO)
	echo "  AR    $@"
	$(AR) -rc $@ $(OBJS_CRYPTO)
	echo "  RL    $@"
	$(AR) -s $@

%.o: %.c
	$(CC) $(LOCAL_CFLAGS) $(CFLAGS) -c $<

clean:
	rm -f *.o libmbed*
