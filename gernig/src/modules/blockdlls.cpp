#include <Windows.h>
#include <processthreadsapi.h>
#include <modules/blockdlls.hpp>

void blockdlls()
{/*
	STARTUPINFOEXA si;
	PROCESS_INFORMATION pi;
	SIZE_T size = 0;
	BOOL ret;

	// Required for a STARTUPINFOEXA
	ZeroMemory(&si, sizeof(si));
	si.StartupInfo.cb = sizeof(STARTUPINFOEXA);
	si.StartupInfo.dwFlags = EXTENDED_STARTUPINFO_PRESENT;

	// Get the size of our PROC_THREAD_ATTRIBUTE_LIST to be allocated
	InitializeProcThreadAttributeList(NULL, 1, 0, &size);

	// Allocate memory for PROC_THREAD_ATTRIBUTE_LIST
	si.lpAttributeList = (LPPROC_THREAD_ATTRIBUTE_LIST)HeapAlloc(
		GetProcessHeap(),
		0,
		size
	);

	// Initialise our list 
	InitializeProcThreadAttributeList(si.lpAttributeList, 1, 0, &size);

	// Enable blocking of non-Microsoft signed DLLs
	DWORD64 policy = PROCESS_CREATION_MITIGATION_POLICY_BLOCK_NON_MICROSOFT_BINARIES_ALWAYS_ON;

	// Assign our attribute
	UpdateProcThreadAttribute(si.lpAttributeList, 0, PROC_THREAD_ATTRIBUTE_MITIGATION_POLICY, &policy, sizeof(policy), NULL, NULL);

	// Finally, create the process
	ret = CreateProcessA(
		NULL,
		(LPSTR)"C:\\Windows\\System32\\cmd.exe",
		NULL,
		NULL,
		true,
		EXTENDED_STARTUPINFO_PRESENT,
		NULL,
		NULL,
		reinterpret_cast<LPSTARTUPINFOA>(&si),
		&pi
	);*/
}