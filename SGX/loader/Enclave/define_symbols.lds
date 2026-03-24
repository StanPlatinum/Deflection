/* 
 * Weijie: 
 * operations for .sgx.code which is inserted using objcopy
 * .sgx.data is defined inside the loader.cpp
 */

SECTIONS
{
    .sgx.code : {
        __elf_start = .;
        *(.sgx.code)
    }
}
__elf_end = .;
