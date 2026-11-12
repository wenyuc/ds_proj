#ifndef STACK_H
#define STACK_H

#include <stdlib.h>

#define MAXSIZE 100
#define SUCCESS 0
#define ERROR 1

typedef int Status;
typedef char ElemType;
typedef enum { FALSE, TRUE } Bool;

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
int static_stackSize(StaticStack *S);

// DynamicStack structure
//typedef void *NewElemType;
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
int dynamic_stackSize(DynamicStack *S);

#endif // STACK_H