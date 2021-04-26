
#if 0
void simple_init()
{
	PrintDebugInfo("simple init\n");
	p_specialname = &psn;
	PrintDebugInfo("value of p_specialname: %p\n", p_specialname);
	psn = 128;
	//p_specialname = 128;
	PrintDebugInfo("value at p_specialname: %d\n", *p_specialname);
	//PrintDebugInfo("value of p_specialname: %d\n", p_specialname);
}

void secure_timer()
{
        thread_data_t *thread_data = get_thread_data();
        shadow_ssa = (ssa_gpr_t *)(thread_data->first_ssa_gpr);
        shadow_rip = &(shadow_ssa->REG(ip));
        shadow_rip_addr = (unsigned long long)&(shadow_ssa->REG(ip));
        while (1) {
                // set SSA marker
                shadow_ssa->REG(ip) = 0;
        }
	//shadow_ssa->REG(ip) = 0;
}
#endif

void main_ssa_init()
{
	unsigned long *main_rip = NULL;
	PrintDebugInfo("ssa init\n");
        main_ssa = (ssa_gpr_t *)(thread_data->first_ssa_gpr);
        main_rip = &(main_ssa->REG(ip));
        main_ssa->REG(ip) = 1;
	PrintDebugInfo("rip in ssa: %d\n", (int)main_ssa->REG(ip));
        //*main_rip = 1;
        //while (shadow_ssa == NULL) __asm __volatile("pause");
}
