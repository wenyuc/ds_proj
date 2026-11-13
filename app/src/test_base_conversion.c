#include <stdio.h>
#include "stack.h"

int main(void)
{
    static const char digits[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    long long input;
    int base;
    unsigned long long value;
    StaticStack *stack;
    char digit;

    if (scanf("%lld %d", &input, &base) != 2) {
        fprintf(stderr, "Enter an integer and a base (2-36).\n");
        return ERROR;
    }
    if (base < 2 || base > 36) {
        fprintf(stderr, "Base must be between 2 and 36.\n");
        return ERROR;
    }

    stack = create_staticStack();
    if (stack == NULL) {
        fprintf(stderr, "Failed to create stack.\n");
        return ERROR;
    }

    value = input < 0
        ? (unsigned long long)(-(input + 1)) + 1
        : (unsigned long long)input;

    do {
        digit = digits[value % (unsigned int)base];
        if (static_push(stack, digit) != SUCCESS) {
            fprintf(stderr, "Stack capacity exceeded.\n");
            destroy_staticStack(stack);
            return ERROR;
        }
        value /= (unsigned int)base;
    } while (value > 0);

    printf("Converted value: ");
    if (input < 0) {
        putchar('-');
    }
    while (static_pop(stack, &digit) == SUCCESS) {
        putchar(digit);
    }
    putchar('\n');

    destroy_staticStack(stack);
    return SUCCESS;
}
