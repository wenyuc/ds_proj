#include <stdio.h>
#include <stdlib.h>

#define MAXSIZE 100
#define SUCCESS 0
#define ERROR 1
#define TRUE 1
#define FALSE 0

typedef char ElemType;
typedef int Status;

typedef struct {
    ElemType *data;
    int top;  //栈顶的位置
}Stack;

Stack* initStack()
{
    Stack *S=(Stack*)malloc(sizeof(Stack));
    S->data=(ElemType*)malloc(sizeof(ElemType) * MAXSIZE);
    S->top=-1;

    return S;
}

int isEmpty(Stack *S)
{
    return (S->top == -1) ? TRUE : FALSE;
}

int isFull(Stack *S)
{
    return (S->top == MAXSIZE-1) ? TRUE : FALSE;
}

//push e into Stack
//return SUCCESS or ERROR
Status push(Stack *S, ElemType e)
{
    if (isFull(S))
    {
        return ERROR;
    } else { //not full
        S->top++;
        S->data[S->top]=e;
    } 
    return SUCCESS;
}

//pop
//Return *e
Status pop(Stack *S, ElemType *e)
{
    if (isEmpty(S))
    {
        return ERROR;
    } else {
        *e=S->data[S->top];
        S->top--;
    }
    return SUCCESS;
}

Status getTop(Stack *S, ElemType *e)
{
    if (isEmpty(S)) return ERROR;
    *e=S->data[S->top];
    return SUCCESS;
}

int main(void)
{
    Stack *S=initStack();
    if (S==NULL) return ERROR;

    push(S, 'A');
    push(S, 'B');
    push(S, 'C');
    ElemType e;
    pop(S, &e);
    printf("pop: %c\n",e);
    pop(S, &e);
    printf("pop: %c\n",e);
    pop(S, &e);
    printf("pop: %c\n",e);
    printf("is Empty: %d\n",isEmpty(S));

    return SUCCESS;
}