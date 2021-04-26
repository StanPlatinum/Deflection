#include "enclave.h"

#include "CFICheck.c"

void sl(){
	int a = 0;
	for(int i = 0; i < 10; i++)
	{
		a++;
	}
}

void enclave_main(){
	int b = 1;
	sl();
	enclave_exit();
}
