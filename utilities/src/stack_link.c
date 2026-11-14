#include <string.h>
#include <stdlib.h>
#include "stack.h"

// Linked Static Stack functions
LinkedStaticStack *create_linkedStaticStack(void)
{
    LinkedStaticStack *S = (LinkedStaticStack *)malloc(sizeof(LinkedStaticStack));
    if (S == NULL)
    {
        return NULL;
    }
    S->next = NULL;
    return S;
}//create_linkedStaticStack

Status init_linkedStaticStack(LinkedStaticStack *S)
{
    if (S == NULL)
    {
        return ERROR;
    }
    S->next = NULL;
    return SUCCESS;
}//init_linkedStaticStack

Status destroy_linkedStaticStack(LinkedStaticStack *S)
{
    if (S == NULL)
    {
        return ERROR;
    }
    free(S);
    return SUCCESS;
}//destroy_linkedStaticStack

Status linkedStatic_push(LinkedStaticStack *S, ElemType e)
{
    if (S == NULL)
    {
        return ERROR;
    }
    LinkedStaticStack *newNode = (LinkedStaticStack *)malloc(sizeof(LinkedStaticStack));
    if (newNode == NULL)
    {
        return ERROR;
    }
    newNode->data[0] = e;
    newNode->next = S->next;
    S->next = newNode;
    return SUCCESS;
}//linkedStatic_push

Status linkedStatic_pop(LinkedStaticStack *S, ElemType *e)
{
    if (S == NULL || S->next == NULL)
    {
        return ERROR;
    }
    LinkedStaticStack *nodeToPop = S->next;
    *e = nodeToPop->data[0];
    S->next = nodeToPop->next;
    free(nodeToPop);
    return SUCCESS;
}//linkedStatic_pop

Status linkedStatic_getTop(LinkedStaticStack *S, ElemType *e)
{
    if (S == NULL || S->next == NULL)
    {
        return ERROR;
    }
    *e = S->next->data[0];
    return SUCCESS;
}//linkedStatic_getTop

Bool linkedStatic_isEmpty(LinkedStaticStack *S)
{
    return (S == NULL || S->next == NULL) ? TRUE : FALSE;
}//linkedStatic_isEmpty

size_t linkedStatic_stackSize(LinkedStaticStack *S)
{
    size_t size = 0;
    LinkedStaticStack *current = S->next;
    while (current != NULL)
    {
        size++;
        current = current->next;
    }
    return size;
}//linkedStatic_stackSize

// Linked Dynamic Stack functions
LinkedDynamicStack *create_linkedDynamicStack(int initialCapacity, int elem_size)
{
    LinkedDynamicStack *S = (LinkedDynamicStack *)malloc(sizeof(LinkedDynamicStack));
    if (S == NULL)
    {
        return NULL;
    }
    S->data = (void **)malloc(initialCapacity * elem_size);
    if (S->data == NULL)
    {
        free(S);
        return NULL;
    }
    S->elem_size = elem_size;
    S->capacity = initialCapacity;
    S->next = NULL;
    return S;
}//create_linkedDynamicStack

Status init_linkedDynamicStack(LinkedDynamicStack *S, int initialCapacity, int elem_size)
{
    if (S == NULL)
    {
        return ERROR;
    }
    S->data = (void **)malloc(initialCapacity * elem_size);
    if (S->data == NULL)
    {
        return ERROR;
    }
    S->elem_size = elem_size;
    S->capacity = initialCapacity;
    S->next = NULL;
    return SUCCESS;
}//init_linkedDynamicStack

Status destroy_linkedDynamicStack(LinkedDynamicStack *S)
{
    if (S == NULL)
    {
        return ERROR;
    }
    free(S->data);
    free(S);
    return SUCCESS;
}//destroy_linkedDynamicStack

Status linkedDynamic_push(LinkedDynamicStack *S, const void *e)
{
    if (S == NULL)
    {
        return ERROR;
    }
    LinkedDynamicStack *newNode = (LinkedDynamicStack *)malloc(sizeof(LinkedDynamicStack));
    if (newNode == NULL)
    {
        return ERROR;
    }
    newNode->data = (void **)malloc(S->capacity * S->elem_size);
    if (newNode->data == NULL)
    {
        free(newNode);
        return ERROR;
    }
    newNode->elem_size = S->elem_size;
    newNode->capacity = S->capacity;
    memcpy((char *)newNode->data + (size_t)0 * S->elem_size, e, S->elem_size);
    newNode->next = S->next;
    S->next = newNode;
    return SUCCESS;
}//linkedDynamic_push

Status linkedDynamic_pop(LinkedDynamicStack *S, void *e)
{
    if (S == NULL || S->next == NULL)
    {
        return ERROR;
    }
    LinkedDynamicStack *nodeToPop = S->next;
    memcpy(e, (char *)nodeToPop->data + (size_t)0 * nodeToPop->elem_size, nodeToPop->elem_size);
    S->next = nodeToPop->next;
    free(nodeToPop->data);
    free(nodeToPop);
    return SUCCESS;
}//linkedDynamic_pop

Status linkedDynamic_getTop(LinkedDynamicStack *S, void *e)
{
    if (S == NULL || S->next == NULL)
    {
        return ERROR;
    }
    memcpy(e, (char *)S->next->data + (size_t)0 * S->next->elem_size, S->next->elem_size);
    return SUCCESS;
}//linkedDynamic_getTop

Bool linkedDynamic_isEmpty(LinkedDynamicStack *S)
{
    return (S == NULL || S->next == NULL) ? TRUE : FALSE;
}//linkedDynamic_isEmpty

size_t linkedDynamic_stackSize(LinkedDynamicStack *S)
{
    size_t size = 0;
    LinkedDynamicStack *current = S->next;
    while (current != NULL)
    {
        size++;
        current = current->next;
    }
    return size;
}//linkedDynamic_stackSize