//#include <stdio.h>

#include "enclave.h"
#include "CFICheck.c"

int fun()
{
    //puts("----------1 in fun----------");
    return 1;
}

void enclave_main()
{
    int (*fp)(void);
    fp = fun;
    //puts("----------1----------");
    int b = fp();
    //puts("----------2----------");
    //char i_b[8];
    //char *ii_b = &i_b;
    //puts("----------3----------");
    //ii_b = my_itoa(b, ii_b, 10);
    //puts("----------4----------");
    //puts(ii_b);
    puts("success!");
    enclave_exit();
}
