#include <stdio.h>
#include <stdlib.h>

#define MAXSIZE 100
#define SUCCESS 0
#define ERROR 1
#define TRUE 1
#define FALSE 0

typedef char ElemType;
typedef int Status;

typedef struct stack{
    ElemType data;
    struct stack *next;
}Stack;

Stack* initStack()
{
    Stack *S=(Stack*)malloc(sizeof(Stack));
    S->data=0;
    S->next=NULL;

    return S;
}

int isEmpty(Stack *S)
{
    return (S->next == NULL) ? TRUE : FALSE;
}

//push e into Stack at head
//return SUCCESS or ERROR
Status push(Stack *S, ElemType e)
{
    Stack *t=(Stack*)malloc(sizeof(Stack));
    t->data=e;
    t->next=S->next;
    S->next=t;

    return SUCCESS;
}

//pop 
//Return *e
Status pop(Stack *S, ElemType *e)
{
    if (!isEmpty(S))
    {
        Stack *t=S->next;
        *e=t->data;
        S->next=t->next;
        free(t);

        return SUCCESS;
    } else {
        return ERROR;
    }
}

Status getTop(Stack *S, ElemType *e)
{
    if (!isEmpty(S))
    {
        *e=S->next->data;
        return SUCCESS;
    } else {
        return ERROR;
    }
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