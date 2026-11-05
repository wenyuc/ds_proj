/*将中缀表达式转化为后缀表达式*/
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

const char expr[] = "x/(i-j)*y";//infix expression,to be converted to postfix

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
}//end of initStack

Bool isEmpty(Stack *S)
{
    return (S->top == -1) ? true:false;
}//end of isEmpty

Bool isFull(Stack *S)
{
    return (S->top == MAXSIZE-1)? true: false;
}//end of isFull

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
}//end of push

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
}//end of pop

Status getTop(Stack *S, ElemType *e)
{
    if (isEmpty(S)) return ERROR;
    *e=S->data[S->top];
    return SUCCESS;
}//end of getTop

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
}//end of getToken

void print_token(ElemType e)
{
    // e 可能是 NUM 的字符，也可能是运算符枚举值
    switch (e) {
        case ADD: printf("+"); break;
        case SUB: printf("-"); break;
        case MUL: printf("*"); break;
        case DIV: printf("/"); break;
        case MOD: printf("%%"); break;
        // LEFT_PARE/RIGHT_PARE 一般不会出现在后缀输出里
        default:  printf("%c", e); break;   // NUM 的字符
    }
    return;
}//end of print_token

//Convert infix expression to postfix expression
int postfix(Stack *S)
{
    int in_stack[]={0,19,12,12,13,13,13,0}; //precedence values for operators in the stack
    int out_stack[]={20,19,12,12,13,13,13,0}; //precedence values for operators outside the stack
    SymbolType token;
    char symbol;//保存运算符或操作数
    int index=0;
    S->top=0;//入栈EOS，作为栈底元素，它优先级最低
    S->data[0]=EOS;
    ElemType e;//保存出栈的内容

    token=getToken(&symbol, &index);
    while(token != EOS)
    {
        if(token == NUM)
        {
            print_token(symbol); //output the operand
        }
        else if(token == LEFT_PARE)
        {
            push(S, token);
        }
        else if(token == RIGHT_PARE)
        {
            while(getTop(S, &e) == SUCCESS && e != LEFT_PARE)
            {
                pop(S, &e);
                print_token(e); //output the operator
            }
            pop(S, &e); //pop the left parenthesis
        }
        else //operator
        {
            while(out_stack[token] <= in_stack[S->data[S->top]])
            {
                pop(S, &e);
                print_token(e); //output the operator
            }
            push(S, token);
        }
        token=getToken(&symbol, &index);
    }
    while(getTop(S, &e) == SUCCESS && e != EOS)
    {
        pop(S, &e);
        print_token(e); //output the remaining operators
    }
    printf("\n");
    return SUCCESS;
}//end of postfix

int main(void)
{
    Stack *S=initStack();
    if (S==NULL) return ERROR;
    //int result;
    postfix(S);
    //printf("expression %s result=%d\n",expr, result);
    return SUCCESS;
}
