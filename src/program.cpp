#include <stdio.h>
#include <iostream>

#define SAMPLEDLL_API __declspec(dllexport)

SAMPLEDLL_API int addNumbers(int a, int b)
{
    return a + b;
}

int main()
{
    printf("Via printf\n");
    std::cout << "Via std::cout" << std::endl;
    return 0;
}