#include <stdio.h>
#include "rtm.h"

void transactionEndBegin()
{
    _xend();
    int i = 0;  //i用来记录abort次数，达到10次程lahf序就exit
    //for (; i < 10; i++)
    for (; i < 20; i++)
    {
        int status = _xbegin();
        if (status == _XBEGIN_STARTED)
        {
            return;
        }
        else
        {//fallback here
	        puts("warning2!");
        }
    }
    exit(-1);
}
