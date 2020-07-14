## Usage

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

