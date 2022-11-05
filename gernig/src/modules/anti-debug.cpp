#include <windows.h>
#include <stdio.h>
#include <modules/anti-debug.hpp>
#include <stdexcept>

void debugger_present () {
    if (IsDebuggerPresent()) {
		printf("debugger detected!\n");
		throw std::out_of_range( "VM Detected\n" );
		exit(1);
	}else{
		printf("no debugger detected!\n");
		//throw std::out_of_range( "No VM Detected\n" );
	}
}