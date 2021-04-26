#include "internal/thread_data.h"
#include "internal/arch.h"

ssa_gpr_t *ssa_inprogram = NULL;

uint64_t result1;
uint64_t result2;

void instrument_function_get_time()
{
	//puts("checking...");
	int re_try_count = 0;
	if (ssa_inprogram->rip != 0 ) {
		// set SSA marker
		ssa_inprogram->REG(ip) = 0;
		if (
			//Weijie: the following lines are useless
				(
				 ((result1>>0) & 0xff) >= 0xe1 ||
				 ((result1>>8) & 0xff) >= 0xe1 ||
				 ((result1>>16) & 0xff) >= 0xe1 ||
				 ((result1>>24) & 0xff) >= 0xe1 ||
				 ((result1>>32) & 0xff) >= 0xe1 ||  
				 ((result1>>40) & 0xff) >= 0xe1 || 
				 ((result1>>48) & 0xff) >= 0xe1 || 
				 ((result1>>56) & 0xff) >= 0xe1 
				) && (
					((result2>>0) & 0xff) >= 0xf2 ||
					((result2>>8) & 0xff) >= 0xf2 ||
					((result2>>16) & 0xff) >= 0xf2 ||
					((result2>>24) & 0xff) >= 0xf2 ||
					((result2>>32) & 0xff) >= 0xf2 || 
					((result2>>40) & 0xff) >= 0xf2 ||
					((result2>>48) & 0xff) >= 0xf2 ||
					((result2>>56) & 0xff) >= 0xf2 
				     )
		   ) {
			if (ssa_inprogram->rip != 0 ) re_try_count = 0;
		} else {
			ssa_inprogram->REG(ip) = 1;
			re_try_count ++;
			if (re_try_count > 22) {
				__asm __volatile("hlt");
			}
		}
	}
	else
		puts("ssa init failed!");
}

