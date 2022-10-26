#include <windows.h>

bool debugger () {
    if (IsDebuggerPresent()) {
		return true;
	}
	else {
		return false;
	}

}