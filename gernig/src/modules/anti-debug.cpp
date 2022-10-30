#include <windows.h>
#include <iostream>

bool debugger_present () {
    if (IsDebuggerPresent()) {
		return true;
	}
	else {
		return false;
	}

};

// int debug_breaker(){
//     __try {
//         DebugBreak();
//     }
//     __except (GetExceptionCode() == EXCEPTION_BREAKPOINT ?
//         EXCEPTION_EXECUTE_HANDLER : EXCEPTION_CONTINUE_SEARCH) {
//         return 0;
//     }
// };
	