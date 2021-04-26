#include <stdlib.h>

//int p_inprogram = 0;
int *p_inprogram = NULL;

void instrument_function_get_time()
{
	//if (p_inprogram == 0)	puts("init failed");
	if (p_inprogram == NULL)	puts("init failed");
	else {
		//Weijie: debugging
		char tmp[20];
		//sprintf(tmp, "%d", p_inprogram);
		sprintf(tmp, "%p", p_inprogram);
		puts(tmp);
		sprintf(tmp, "%d", *p_inprogram);
		puts(tmp);
	}
}
