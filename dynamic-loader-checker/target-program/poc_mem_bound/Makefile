LLVM_PATH = /usr/local/src/PoC/proofGen
CFIHello_So_PATH = $(LLVM_PATH)/build/lib/LLVMCFIHello.so
CC = $(LLVM_PATH)/build/bin/clang -fPIC -fno-asynchronous-unwind-tables -fno-addrsig -mstackrealign

CC4CFICheck = $(LLVM_PATH)/build/bin/clang -fPIC -fno-asynchronous-unwind-tables -fno-addrsig -mstackrealign -Xclang -load -Xclang $(CFIHello_So_PATH)

all: poc

poc.s: poc.c
	$(CC4CFICheck) -S -I. $< -o $@

poc: poc.s
	$(CC) $< -o $@

clean:
	rm -f poc poc.s
