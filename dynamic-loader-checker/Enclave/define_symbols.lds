SECTIONS
{
    /* Weijie: add 2 new sections */
    .sgx.ssblob : {
        __ss_start = .;
        *(.sgx.ssblob)    
    }
    .sgx.calltg : {
        __cfi_start = .;
        *(.sgx.calltg)    
    }
    .sgx.code : {
        __elf_start = .;
        *(.sgx.code)
    }
}
__elf_end = .;
