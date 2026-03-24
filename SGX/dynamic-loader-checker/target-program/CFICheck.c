#include <stdio.h>
#include "CFICheck.h"

void CFICheck(unsigned long long target)
{
	unsigned long long *CFICheckAddressPtr = (unsigned long long *)0x1FFFFFFFFFFFFFFFULL;
    int CFICheckAddressNum = 0x1FFFFFFF;
	int low = 0, high = CFICheckAddressNum, mid=0;

    if (target == 0)
    {
        return;
    }
    

	while (low <= high)
	{
		mid = low + (high - low) / 2;
		if (mid > high)
		{
			break;
		}
		if (CFICheckAddressPtr[mid] > target)
		{
			high = mid - 1;
		}
		else if (CFICheckAddressPtr[mid] < target)
		{
			low = mid + 1;
		}
        else
        {
			return;
        }
        
	}
	exit(-1);
}
