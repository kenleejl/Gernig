#include <windows.h>
#include <stdio.h>
#include <modules/anti-debug.hpp>

void debugger_present () {
    if (IsDebuggerPresent()) {
		// printf("debugger detected!\n");
		exit(1);
	}else{
		// printf("no debugger.\n");
	}
}