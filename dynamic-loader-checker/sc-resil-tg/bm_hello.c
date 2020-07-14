/* a very simple test program */

#include <stdio.h>
#include <stdlib.h>

#include "CFICheck.h"
//#include "CFICheck.c"
#include "enclave.h"
//#include "foo1.h"

#if 0
void useless()
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
#endif

void enclave_main()
{
	//unsigned long long magic = 1;
    puts("hello");
    puts("hello2345678906543234567896543456789865123456789076543212345678wedf2345tdgg1n38n9943y284yr98qnyr8yr918y34983y8n4yr832y4n28693y494b9b3yb8fyrf29rf2rg62rf27rg");
    enclave_exit();
}
