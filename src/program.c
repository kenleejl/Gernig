#include <stdio.h>

#define SAMPLEDLL_API __declspec(dllexport)

SAMPLEDLL_API int addNumbers(int a, int b)
{
    return a + b;
}

int main()
{
    printf("Via printf\n");
    return 0;
}