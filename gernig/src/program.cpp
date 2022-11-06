#include <stdio.h>
#include <windows.h>

int main()
{
    for (int i = 0; i < 600; i++)
    {
        printf("Program running for %ds...\n", i);
        Sleep(1000);
    }

    return 0;
}