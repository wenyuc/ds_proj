#include <stdio.h>
#include "stack.h"

int main(void)
{
    //int ret;

    printf("=== test Sequence Static Stack ===\n");
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
    printf("Stack Size: %zu\n", static_stackSize(S_Stack));

    printf("=== test Sequence Dynamic Stack with char data type ===\n");
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
    printf("Stack Size: %zu\n", dynamic_stackSize(D_stack));
    dynamic_getTop(D_stack, &e_char);
    printf("getTop: %c\n",e_char);
    printf("is Empty: %d is Full: %d\n",dynamic_isEmpty(D_stack), dynamic_isFull(D_stack));
    destroy_dynamicStack(D_stack);

    printf("=== test Sequence Dynamic Stack with int data type ===\n");
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
    printf("Stack Size: %zu\n", dynamic_stackSize(D_int_stack));
    dynamic_getTop(D_int_stack, &e_int);
    printf("getTop: %d\n",e_int);
    printf("is Empty: %d is Full: %d\n",dynamic_isEmpty(D_int_stack), dynamic_isFull(D_int_stack));
    destroy_dynamicStack(D_int_stack);

    //Create a Linked Static Stack
    printf("=== test linked static stack ===\n");
    LinkedStaticStack *LS_stack;
    LS_stack = create_linkedStaticStack();
    if (LS_stack == NULL) return ERROR;

    linkedStatic_push(LS_stack, 'A');
    linkedStatic_push(LS_stack, 'B');
    linkedStatic_push(LS_stack, 'C');
    linkedStatic_push(LS_stack, 'D');

    linkedStatic_pop(LS_stack, &e);
    printf("pop: %c\n",e);
    linkedStatic_pop(LS_stack, &e);
    printf("pop: %c\n",e);
    linkedStatic_pop(LS_stack, &e);
    printf("pop: %c\n",e);
    printf("is Empty: %d\n",linkedStatic_isEmpty(LS_stack));

    linkedStatic_getTop(LS_stack, &e);
    printf("getTop: %c\n",e);
    printf("Stack Size: %zu\n", linkedStatic_stackSize(LS_stack));

    destroy_linkedStaticStack(LS_stack);

    printf("=== test linked dynamic stack with char data type ===\n");
    LinkedDynamicStack *LD_char_stack;
    LD_char_stack = create_linkedDynamicStack(initialCapacity, sizeof(char));
    if (LD_char_stack == NULL) return ERROR;

    linkedDynamic_push(LD_char_stack, &a);
    linkedDynamic_push(LD_char_stack, &b);
    linkedDynamic_push(LD_char_stack, &c);
    linkedDynamic_push(LD_char_stack, &d);

    linkedDynamic_pop(LD_char_stack, &e_char);
    printf("pop: %c\n",e_char);
    linkedDynamic_pop(LD_char_stack, &e_char);
    printf("pop: %c\n",e_char);
    linkedDynamic_pop(LD_char_stack, &e_char);
    printf("pop: %c\n",e_char);
    printf("is Empty: %d\n",linkedDynamic_isEmpty(LD_char_stack));

    linkedDynamic_getTop(LD_char_stack, &e_char);
    printf("getTop: %c\n",e_char);
    printf("Stack Size: %zu\n", linkedDynamic_stackSize(LD_char_stack));

    destroy_linkedDynamicStack(LD_char_stack);

    printf("=== test Linked Dynamic Stack with int data type ===\n");
    LinkedDynamicStack *LD_int_stack;
    LD_int_stack = create_linkedDynamicStack(initialCapacity, sizeof(int));
    if (LD_int_stack == NULL) return ERROR;

    linkedDynamic_push(LD_int_stack, &u);
    linkedDynamic_push(LD_int_stack, &v);
    linkedDynamic_push(LD_int_stack, &w);
    linkedDynamic_push(LD_int_stack, &x);

    linkedDynamic_pop(LD_int_stack, &e_int);
    printf("pop: %d\n",e_int);
    linkedDynamic_pop(LD_int_stack, &e_int);
    printf("pop: %d\n",e_int);
    linkedDynamic_pop(LD_int_stack, &e_int);
    printf("pop: %d\n",e_int);
    printf("is Empty: %d\n",linkedDynamic_isEmpty(LD_int_stack));

    linkedDynamic_getTop(LD_int_stack, &e_int);
    printf("getTop: %d\n",e_int);
    printf("Stack Size: %zu\n", linkedDynamic_stackSize(LD_int_stack));

    destroy_linkedDynamicStack(LD_int_stack);
    
    return SUCCESS;
}