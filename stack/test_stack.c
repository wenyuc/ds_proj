#include <stdio.h>

#define MAXSIZE 100
#define SUCCESS 0
#define ERROR 1
#define TRUE 1
#define FALSE 0

typedef char ElemType;
typedef int Status;

typedef struct {
    ElemType data[MAXSIZE];
    int top;  //栈顶的位置，即data[top]
}Stack;

Status initStack(Stack *S)
{
    S->top=-1;
    return SUCCESS;
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

int main(void)
{
    int ret;
    Stack S;
    ret = initStack(&S);
    if (ret!=SUCCESS) return ERROR;
    
    push(&S, 'A');
    push(&S, 'B');
    push(&S, 'C');
    ElemType e;
    pop(&S, &e);
    printf("pop: %c\n",e);
    pop(&S, &e);
    printf("pop: %c\n",e);
    pop(&S, &e);
    printf("pop: %c\n",e);
    printf("is Empty: %d\n",isEmpty(&S));

    return SUCCESS;
}