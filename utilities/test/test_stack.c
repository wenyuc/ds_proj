#include <stdio.h>
#include "stack.h"

int main(void)
{
    //int ret;

    printf("--- create a StaticStack---\n");
    StaticStack* S_Stack;
    S_Stack=create_staticStack();
    if (S_Stack==NULL) return ERROR;
    
    static_push(S_Stack, 'A');
    static_push(S_Stack, 'B');
    static_push(S_Stack, 'C');
    static_push(S_Stack, 'D');

    ElemType e;
    static_pop(S_Stack, &e);
    printf("pop: %c\n",e);
    static_pop(S_Stack, &e);
    printf("pop: %c\n",e);
    static_pop(S_Stack, &e);
    printf("pop: %c\n",e);
    printf("is Empty: %d is Full: %d\n",static_isEmpty(S_Stack), static_isFull(S_Stack));

    static_getTop(S_Stack, &e);
    printf("getTop: %c\n",e);
    printf("Stack Size: %d\n", static_stackSize(S_Stack));

    printf("--- create a DynamicStack with char data type---\n");
    DynamicStack *D_stack;
    int initialCapacity=10;
    char e_char;
    D_stack = create_dynamicStack(initialCapacity, sizeof(char));
    char a='W';char b='X';char c='Y';char d='Z';
    dynamic_push(D_stack, &a);
    dynamic_push(D_stack, &b);
    dynamic_push(D_stack, &c);
    dynamic_push(D_stack, &d);
    dynamic_pop(D_stack, &e_char);
    printf("pop: %c\n",e_char);
    dynamic_pop(D_stack, &e_char);
    printf("pop: %c\n", e_char);
    printf("Stack Size: %d\n", dynamic_stackSize(D_stack));
    dynamic_getTop(D_stack, &e_char);
    printf("getTop: %c\n",e_char);
    printf("is Empty: %d is Full: %d\n",dynamic_isEmpty(D_stack), dynamic_isFull(D_stack));
    destroy_dynamicStack(D_stack);

    printf("--- create a DynamicStack with int data type---\n");
    DynamicStack *D_int_stack;
    int e_int;
    initialCapacity=5;
    D_int_stack = create_dynamicStack(initialCapacity, sizeof(int));
    int u=17;int v=18; int w=19; int x=20;
    dynamic_push(D_int_stack, &u);
    dynamic_push(D_int_stack, &v);
    dynamic_push(D_int_stack, &w);
    dynamic_push(D_int_stack, &x);
    dynamic_pop(D_int_stack, &e_int);
    printf("pop: %d\n",e_int);
    dynamic_pop(D_int_stack, &e_int);
    printf("pop: %d\n",e_int);
    printf("Stack Size: %d\n", dynamic_stackSize(D_int_stack));
    dynamic_getTop(D_int_stack, &e_int);
    printf("getTop: %d\n",e_int);
    printf("is Empty: %d is Full: %d\n",dynamic_isEmpty(D_int_stack), dynamic_isFull(D_int_stack));
    destroy_dynamicStack(D_int_stack);

    return SUCCESS;
}