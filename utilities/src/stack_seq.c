/*
Static and Dynamic Stack Operations Implementation
*/
#include <string.h>
#include <stdlib.h>
#include "stack.h"

//StaticStack functions
//initialize a stack by setting its top to -1.
Status init_staticStack(StaticStack *S)
{
    S->top=-1;
    return SUCCESS;
}//initStack

//create a new stack from heap and initialize it.
StaticStack *create_staticStack(void)
{
    StaticStack *S = (StaticStack *)malloc(sizeof(StaticStack));
    if (S == NULL)
    {
        return NULL;
    }
    S->top = -1;
    return S;
}//createStack

Bool static_isEmpty(StaticStack *S)
{
    return (S->top == -1) ? TRUE : FALSE;
}//isEmpty

Bool static_isFull(StaticStack *S)
{
    return (S->top == MAXSIZE-1) ? TRUE : FALSE;
}//isFull

size_t static_stackSize(StaticStack *S)
{
    return (size_t)(S->top + 1);
}//stackSize

//push e into Stack
//return SUCCESS or ERROR
Status static_push(StaticStack *S, ElemType e)
{
    if (static_isFull(S))
    {
        return ERROR;
    } else { //not full
        S->top++;
        S->data[S->top]=e;
    } 
    return SUCCESS;
}//push

//pop
//Return *e
Status static_pop(StaticStack *S, ElemType *e)
{
    if (static_isEmpty(S))
    {
        return ERROR;
    } else {
        *e=S->data[S->top];
        S->top--;
    }
    return SUCCESS;
}//pop

Status static_getTop(StaticStack *S, ElemType *e)
{
    if (static_isEmpty(S))
    {
        return ERROR;
    } else {
        *e=S->data[S->top];
    }
    return SUCCESS;
}//getTop

Status destroy_staticStack(StaticStack *S)
{
    if (S == NULL)
    {
        return ERROR;
    }
    free(S);
    return SUCCESS;
}//destroyStack

//DynamicStack functions

//initialize a dynamic stack by setting its top to -1 and allocating initial capacity.
Status init_dynamicStack(DynamicStack *S, int initialCapacity, int elem_size)
{
    S->top = -1;
    S->capacity = initialCapacity;
    S->elem_size = elem_size;
    S->data = (void **)malloc(S->capacity * elem_size);
    if (S->data == NULL)
    {
        return ERROR;
    }
    return SUCCESS;
}//init_dynamicStack

//create a dynamic stack by setting its top to -1 and allocating initial capacity.
DynamicStack *create_dynamicStack(int initialCapacity, int elem_size)
{
    DynamicStack *S=(DynamicStack*)malloc(sizeof(DynamicStack));
    if (S == NULL)
    {
        return NULL;
    }
    S->top = -1;
    S->capacity = initialCapacity;
    S->elem_size = elem_size;
    S->data = (void **)malloc(S->capacity * elem_size);
    if (S->data == NULL)
    {
        free(S);
        return NULL;
    }
    return S;
}//create_dynamicStack

Status destroy_dynamicStack(DynamicStack *S)
{
    if (S == NULL)
    {
        return ERROR;
    }
    free(S->data);
    free(S);
    return SUCCESS;
}//destroy_dynamicStack

Bool dynamic_isEmpty(DynamicStack *S)
{
    return (S->top == -1) ? TRUE : FALSE;
}//dynamic_isEmpty

Bool dynamic_isFull(DynamicStack *S)
{
    return (S->top == S->capacity-1) ? TRUE : FALSE;
}//dynamic_isFull

size_t dynamic_stackSize(DynamicStack *S)
{
    return (size_t)(S->top + 1);
}//dynamic_stackSize

Status dynamic_push(DynamicStack *S, const void *e)
{
    if (dynamic_isFull(S))
    {
        return ERROR;
    } else { //not full
        S->top++;
        memcpy((char *)S->data + (size_t)S->top * S->elem_size, e, S->elem_size);
    } 
    return SUCCESS;
}//dynamic_push

Status dynamic_pop(DynamicStack *S, void *e)
{
    if (dynamic_isEmpty(S))
    {
        return ERROR;
    } else {
        memcpy(e, (char *)S->data + (size_t)S->top * S->elem_size, S->elem_size);
        S->top--;
    }
    return SUCCESS;
}//dynamic_pop

Status dynamic_getTop(DynamicStack *S, void *e)
{
    if (dynamic_isEmpty(S))
    {
        return ERROR;
    } else {
        memcpy(e, (char *)S->data + (size_t)S->top * S->elem_size, S->elem_size);
    }
    return SUCCESS;
}//dynamic_getTop
