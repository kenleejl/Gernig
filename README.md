# Gernig

## Basic Usage

The following example shows how to add basic noise via the Python interface:

```python
from gernig import Noiser, PrintNoise

n = Noiser("program64.exe") # Initialize with target binary
n.addNoise(PrintNoise("Hello world!")) # Add PrintNoise module
n.generate("output.exe")
```

Run the generated binary:

```
.\output.exe
```

## Development Setup

### Prerequisites

- [MinGW-w64 multilib](https://sourceforge.net/projects/mingw-w64/files/Toolchains%20targetting%20Win64/Personal%20Builds/mingw-builds/8.1.0/threads-posix/sjlj/x86_64-8.1.0-release-posix-sjlj-rt_v6-rev0.7z/download)
- [GnuWin Make](http://gnuwin32.sourceforge.net/downlinks/make.php)
- Python 3

## Gernig Development

- `loader32/64.exe` - Main program that will dispatch noise modules as threads and load the target binary from memory
- `program32/64.exe` - Sample target program used for testing, simulates a simple program loop

To develop and test core Gernig functionality outside of Python, build via Make:

```
cd gernig
make
```

Build for 32-bit:

```
make m32=1
```

Run on sample program:

```
loader64.exe program64.exe
```

### Module Development

Each noise module is separated into their respective `.cpp` and `.hpp` files located in `gernig/src/modules` and `gernig/include/modules` respectively.

Modules are implemented as thread callback functions passed to `std::thread`, which will be dispatched from `main`.

In the following module function, the `printLoop` function will be launched as a thread and the argument `msg` will be printed to the console every second, alongside the target binary.

```cpp
void printLoop(std::string msg)
{
    while (1)
    {
        std::cout << msg << std::endl;
        Sleep(1000);
    }
}
```

Preprocessor conditional directives are used to "pass information" from Python to C++, such as enabling a certain module or passing a string parameter:

`defines.h`:

```c
#define _PRINT_NOISE_ENABLED
#define _PRINT_NOISE_TEXT "Hello world!"
```

`main.cpp`:

```cpp
#include <defines.h>

#ifdef _PRINT_NOISE_ENABLED
    std::thread t1(printLoop, _PRINT_NOISE_TEXT);
#endif
```

Finally, the original target binary is embedded into the program as an array of `unsigned char` in `binexp.h`, and is parsed and loaded directly from memory via `LoadFromMemory` by calling its entry point stored in `pMemoryModule->exeEntry`.
Further obfuscation can be performed on this array if required.

`binexp.h`:

```c
unsigned char BINARY_ARRAY[] = {
    0x4d, 0x5a, 0x90, 0x00, 0x03,
    ...
}
```
