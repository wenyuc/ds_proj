/*
Stack data structures and their operations.
*/
#ifndef STACK_H
#define STACK_H

#include <stddef.h>
#include "ds_type.h"

//StaticStack structure
typedef struct {
    ElemType data[MAXSIZE];
    int top;  // Stack top, pop the element at this position
}StaticStack; // Element type is char, fixed size stack.

// StaticStack functions
StaticStack *create_staticStack(void);
Status init_staticStack(StaticStack *S);
Status destroy_staticStack(StaticStack *S);
Status static_push(StaticStack *S, ElemType e);
Status static_pop(StaticStack *S, ElemType *e);
Status static_getTop(StaticStack *S, ElemType *e);
Bool static_isEmpty(StaticStack *S);
Bool static_isFull(StaticStack *S);
size_t static_stackSize(StaticStack *S);

// DynamicStack structure
typedef struct {
    void **data; //any data type can be stored in the stack
    size_t elem_size; //size of each element in the stack
    int capacity; //current allocated size of the stack
    int top;  //stack top, pop the element at this position
}DynamicStack;

// DynamicStack functions
DynamicStack *create_dynamicStack(int initialCapacity, int elem_size);
Status init_dynamicStack(DynamicStack *S, int initialCapacity, int elem_size);
Status destroy_dynamicStack(DynamicStack *S);
Status dynamic_push(DynamicStack *S, const void *e);
Status dynamic_pop(DynamicStack *S, void *e);
Status dynamic_getTop(DynamicStack *S, void *e);
Bool dynamic_isEmpty(DynamicStack *S);
Bool dynamic_isFull(DynamicStack *S);
size_t dynamic_stackSize(DynamicStack *S);

/*
Linked Static Stack Structure
data type is decided by ElemType. Sequence of elements is stored in a linked manner.
*/
typedef struct stack{
    ElemType data[MAXSIZE];
    struct stack *next;
}LinkedStaticStack;

// Linked Static Stack functions
LinkedStaticStack *create_linkedStaticStack(void);
Status init_linkedStaticStack(LinkedStaticStack *S);
Status destroy_linkedStaticStack(LinkedStaticStack *S);
Status linkedStatic_push(LinkedStaticStack *S, ElemType e);
Status linkedStatic_pop(LinkedStaticStack *S, ElemType *e);
Status linkedStatic_getTop(LinkedStaticStack *S, ElemType *e);
Bool linkedStatic_isEmpty(LinkedStaticStack *S);
size_t linkedStatic_stackSize(LinkedStaticStack *S);

//Linked Dynamic Stack Structure
typedef struct ld_stack {
    void **data;
    size_t elem_size;
    int capacity;
    struct ld_stack *next;
}LinkedDynamicStack;

// Linked Dynamic Stack functions
LinkedDynamicStack *create_linkedDynamicStack(int initialCapacity, int elem_size);
Status init_linkedDynamicStack(LinkedDynamicStack *S, int initialCapacity, int elem_size);
Status destroy_linkedDynamicStack(LinkedDynamicStack *S);
Status linkedDynamic_push(LinkedDynamicStack *S, const void *e);
Status linkedDynamic_pop(LinkedDynamicStack *S, void *e);
Status linkedDynamic_getTop(LinkedDynamicStack *S, void *e);
Bool linkedDynamic_isEmpty(LinkedDynamicStack *S);
//Bool linkedDynamic_isFull(LinkedDynamicStack *S);
size_t linkedDynamic_stackSize(LinkedDynamicStack *S);

#endif // STACK_H
