# include <windows.h>
#include <iostream>

bool asm_detection () {
	CONTEXT Ctx = { 0 };
	HANDLE hHandle = GetCurrentThread();

	Ctx.ContextFlags = CONTEXT_DEBUG_REGISTERS;

	if (GetThreadContext(hHandle, &Ctx)) {
		if ((Ctx.Dr0 != 0x00) || (Ctx.Dr1 != 0x00) || (Ctx.Dr2 != 0x00) || (Ctx.Dr3 != 0x00) || (Ctx.Dr6 != 0x00) || (Ctx.Dr7 != 0x00)) {
			return true;
			exit(0x2000); //Exceeds CPU RANGE
	}

	return false;

	}
}

#include <stdio.h>
//https://github.com/TheDuchy/rdtsc-cpuid-vm-check/blob/master/main.c
#ifdef _WIN32 
    #include <windows.h>
    #include <intrin.h>
    #pragma intrinsic(__rdtsc)
#endif

// bool rdtsc_cpuid_check(){
//     unsigned long long int time1, time2, sum = 0;
//     const unsigned char avg = 100;
    
//     for (int i = 0; i < avg; i++){
//         time1 = __rdtsc();
// #ifdef _WIN32
//         __asm _cpuid;
// #endif
//         time2 = __rdtsc();
//         sum += time2 - time1;
//     }

//     sum = sum / avg;
    
//     printf("Ticks on average: %llu\n", sum);

//     if(sum > 500){
// 		// VM
//         return true;
//     }else{
// 		// Bare Metal
//         return false;
//     }
// }