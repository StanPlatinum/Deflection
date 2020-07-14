#Weijie: add makefile rules for objs

#all: libc_targetprogram.a

#Weijie: extract exit.o
libc_targetprogram.a: ./libc.a
	$(AR) -x ./libc.a
	$(AR) -rs libc_targetprogram.a *.o ../enclave_main.o

libc_sslserver.a: ./libc.a
	$(AR) -x ./libc.a
	$(AR) -rs libc_sslserver.a *.o ../ssl_server.o

clean:
	rm -f *.o *.lo *.a

