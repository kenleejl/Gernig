#include <stdio.h>
#include <windows.h>

// Unnecessary
#define SAMPLEDLL_API __declspec(dllexport)
SAMPLEDLL_API int addNumbers(int a, int b)
{
    return a + b;
}

int main()
{
    for (int i = 0; i < 600; i++)
    {
        printf("Program running for %ds...\n", i);
        Sleep(1000);
    }

    return 0;
}