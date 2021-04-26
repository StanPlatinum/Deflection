//#include <stdio.h>
#include <stdlib.h>

#include "enclave.h"

#include "CFICheck.c"

#define MSIZE 0x1000

void sort(unsigned long *objs, unsigned long *objs_end, unsigned int left, unsigned int right)
{
    unsigned int i, j;
    unsigned long pivot, tmp;
    if ((int)left >= (int)right) return; // potentially under-flow!
    pivot = objs[left];
    i = left;
    for (j = left + 1;j <= right; ++j) {
        if (objs[j] < pivot) {
            ++i;
            tmp = objs[j]; objs[j] = objs[i]; objs[i] = tmp;
            tmp = objs_end[j]; objs_end[j] = objs_end[i]; objs_end[i] = tmp;
        }
    }
    tmp = objs[left]; objs[left] = objs[i]; objs[i] = tmp;
    tmp = objs_end[left]; objs_end[left] = objs_end[i]; objs_end[i] = tmp;
    sort(objs, objs_end, left, i-1);
    sort(objs, objs_end, i+1, right);
}

int check_overlap(unsigned long *objs, unsigned long *objs_end)
{
	puts("test check_overlap");
    unsigned int i, j=0;
    for (i = 1;i < MSIZE;++i) {
        if (objs_end[i-1] > objs[i])	return 1;
	//Weijie: test
	j++;
    }
    return 0;
}

char __tmp[64] = {0};
#define dlog(...) sprintf(__tmp, __VA_ARGS__); puts(__tmp); memset(__tmp, 0, 64)


void enclave_main()
{
	puts("test0");
    
	unsigned int i;
    unsigned long *objs = (unsigned long*)malloc(MSIZE * sizeof(unsigned long));
	puts("test1");
    unsigned long *objs_end = (unsigned long*)malloc(MSIZE * sizeof(unsigned long));
	
	for (i = 0;i < MSIZE; ++i) {
        size_t size = i % 5 + 5;
        objs[i] = (unsigned long)malloc(size);
        objs_end[i] = objs[i]+size;
    }
	
	sort(objs, objs_end, 0, MSIZE-1);
	
	puts("test2");
    
	if (check_overlap(objs, objs_end))
        puts("overlap exist");
    else
        puts("no overlap");

    for (i = 0;i < MSIZE; ++i)
        free((void*)objs[i]);

	puts("success!");
    enclave_exit();
}
