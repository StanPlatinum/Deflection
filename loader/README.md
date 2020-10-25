## Usage

Please be noted that, using proofGen/tsx to generate code + proof w/ TSX, using proofGen/master to generate code + proof w/o TSX.

***

### Basic usage

1. build target relocatable elf

cd target-program && make

2. build sgx application 

cd .. && make

### Optional commands:

1. generate symtab debugging tool

make symtab_test

2. run the tool

./symtab_test $(TARGET_NAME)

------------------------------------
Memory Layouts:
------------------------------------

__elf_start	(Here begins the .sgx.code: 64M)

32M memory space for other section

_SGXCODE_BASE

32M memory space for received binary

__elf_end, __HEAP_BASE
(HEAP BASE would be a slightly larger than elf end.)

0x27000 Bytes space for loader's own heap

_SGXDATA_BASE

some space for .data section from received binary

...
