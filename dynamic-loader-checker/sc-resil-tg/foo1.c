#include <stdio.h>
#include "CFICheck.h"

void useless1()
{
    int a = 0, b = 1;
    if (a < b)
    {
        CFICheck(0);
    }
    else
    {
        exit(0);
    }
    return;
}

int foo1()
{
    puts("foo1");
    return 100;
}
