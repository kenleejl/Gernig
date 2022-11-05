#define WINVER 0x0A00
#define _WIN32_WINNT 0x0A00

#include <iostream>
#include <Windows.h>
#include <processthreadsapi.h>
#include <tchar.h>
#include <modules/acg.hpp>

void acg()
{/*
	STARTUPINFOEX si;
	DWORD oldProtection;
	
	PROCESS_MITIGATION_DYNAMIC_CODE_POLICY policy;
	ZeroMemory(&policy, sizeof(policy));
	policy.ProhibitDynamicCode = 1;

	void* mem = VirtualAlloc(0, 1024, MEM_RESERVE | MEM_COMMIT, PAGE_EXECUTE_READWRITE);
	if (mem == NULL) {
		printf("[!] Error allocating RWX memory\n");
	}
	else {
		printf("[*] RWX memory allocated: %p\n", mem);
	}

	printf("[*] Now running SetProcessMitigationPolicy to apply PROCESS_MITIGATION_DYNAMIC_CODE_POLICY\n");
	
	// Set our mitigation policy
	if (SetProcessMitigationPolicy(ProcessDynamicCodePolicy, &policy, sizeof(policy)) == false) {
		printf("[!] SetProcessMitigationPolicy failed\n");
		exit(1);
	}

	// Attempt to allocate RWX protected memory (this will fail)
	mem = VirtualAlloc(0, 1024, MEM_RESERVE | MEM_COMMIT, PAGE_EXECUTE_READWRITE);
	if (mem == NULL) {
		printf("[!] Error allocating RWX memory\n");
	}
	else {
		printf("[*] RWX memory allocated: %p\n", mem);
	}

	void* ntAllocateVirtualMemory = GetProcAddress(LoadLibraryA("ntdll.dll"), "NtAllocateVirtualMemory");

	// Let's also try a VirtualProtect to see if we can update an existing page to RWX
	if (!VirtualProtect(ntAllocateVirtualMemory, 4096, PAGE_EXECUTE_READWRITE, &oldProtection)) {
		printf("[!] Error updating NtAllocateVirtualMemory [%p] memory to RWX\n", ntAllocateVirtualMemory);
	}
	else {
		printf("[*] NtAllocateVirtualMemory [%p] memory updated to RWX\n", ntAllocateVirtualMemory);
	}*/
}