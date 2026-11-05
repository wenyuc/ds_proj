#include <stdio.h>
#include <stdlib.h>

#define MAXSIZE 100
#define SUCCESS 0
#define ERROR 1
typedef int ElemType;
typedef int Status;

typedef struct{
    ElemType *data;
    int top;
}Stack;

typedef enum {
    LEFT_PARE, RIGHT_PARE,
    ADD, SUB, MUL, DIV,MOD,
    EOS, NUM
}SymbolType;

typedef enum{false,true}Bool;

const char expr[] = "82/2+56*-";

//Init Stack
Stack* initStack(void)
{
    Stack *S=(Stack*)malloc(sizeof(Stack));
    if (S==NULL) return NULL;
    
    S->data=(ElemType*)malloc(sizeof(ElemType) * MAXSIZE);
    if(S->data==NULL) {
        free(S);
        return NULL;
    }
    
    S->top=-1;
    return S;
}

Bool isEmpty(Stack *S)
{
    return (S->top == -1) ? true:false;
}

Bool isFull(Stack *S)
{
    return (S->top == MAXSIZE-1)? true: false;
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

SymbolType getToken(char *symbol, int *index)
{
    *symbol=expr[*index];
    *index = *index +1;
    switch(*symbol)
    {
        case '(':
            return LEFT_PARE;
        case ')':
            return RIGHT_PARE;
        case '+':
            return ADD;
        case '-':
            return SUB;
        case '*':
            return MUL;
        case '/':
            return DIV;
        case '%':
            return MOD;
        case '\0':
            return EOS;
        default:
            return NUM;
    }
}

int eval(Stack *S)
{
    char symbol;
    int op1,op2;
    int index=0;
    SymbolType token;
    ElemType result;
    token=getToken(&symbol, &index);
    while(token != EOS)
    {
        if (token==NUM)
        {
            push(S, symbol-'0');
        } else{
            pop(S, &op2);
            pop(S, &op1);
            
            switch(token)
            {
                case ADD:
                    push(S, op1 + op2);
                    break;
                case SUB:
                    push(S, op1 - op2);
                    break;
                case MUL:
                    push(S, op1 * op2);
                    break;
                case DIV:
                    push(S, op1 / op2);
                    break;
                case MOD:
                    push(S, op1 % op2);
                    break;
                default:
                    break;
            }
        }
        token=getToken(&symbol, &index);
    }
    pop(S,&result);
    return result;
}

int main(void)
{
    Stack *S=initStack();
    if (S==NULL) return ERROR;
    int result;
    result=eval(S);
    printf("expression %s result=%d\n",expr, result);
    return SUCCESS;
}
