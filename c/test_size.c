/*
Verify various data type size.
*/

#include <stdint.h>
#include <stdio.h>
#include <stddef.h>

int main(void)
{
    printf("sizeof(uint8_t) = %zu\n", sizeof(uint8_t));   // 1
    printf("sizeof(uint16_t) = %zu\n", sizeof(uint16_t));   // 2
    printf("sizeof(size_t)  = %zu\n", sizeof(size_t));    // 8
    printf("sizeof(int)     = %zu\n", sizeof(int));       // 4（x86_64 常见）
    printf("sizeof(void*)   = %zu\n", sizeof(void *));    // 8

    printf("sizeof(short) = %zu\n", sizeof(short));   // 2
    printf("sizeof(long) = %zu\n", sizeof(long));   // 8
    printf("sizeof(long long) = %zu\n", sizeof(long long));   // 8

    return 0;
}