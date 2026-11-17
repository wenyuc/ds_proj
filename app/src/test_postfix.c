/*将中缀表达式转化为后缀表达式*/
#include <stdio.h>
#include <stdlib.h>
#include "stack.h"
#include "ds_type.h"

//const char expr[] = "x/(i-j)*y";//infix expression,to be converted to postfix
//const char expr[] = "x/(i-j)";
//const char *exprs[]={"x/(i-j)", "x/(i-j)*y"};
//const char expr[]="(A+B*(C-D))/E";//postfix: ABCD-*+E/
//const char expr[]="A-B-C-D";//postfix: AB-C-D-
//const char expr[]="((A+B)*C-(D-E))/F";//postfix: AB+C*DE--F/
//const char expr[]="(A+B)*(C-D)";//postfix:AB+CD-*
const char expr[]="A+B*C-D/E";//postfix: ABC*+DE/-

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

static char token_to_char(SymbolType e)
{
    switch (e) {
        case ADD: return '+';
        case SUB: return '-';
        case MUL: return '*';
        case DIV: return '/';
        case MOD: return '%';
        case LEFT_PARE: return '(';
        case RIGHT_PARE: return ')';
        case EOS: return '\0';
        default: return (char)e;   // NUM 的字符
    }
}

//Convert infix expression to postfix expression
int postfix(DynamicStack *S, char *post, size_t post_capacity)
{
    const int in_stack[]={0,19,12,12,13,13,13,0};//precedence values for operators in the stack
    const int out_stack[]={20,19,12,12,13,13,13,0};//precedence values for operators outside the stack
    SymbolType token;
    char symbol;//保存运算符或操作数
    int index=0;//infix expression index
    size_t post_index=0;//postfix expression index
    int expect_operand=1;//flag to indicate whether an operand is expected next
    SymbolType e=EOS;//save pop value from stack

    if (S == NULL || post == NULL || post_capacity == 0) {
        return ERROR;
    }
    token=EOS;
    if (dynamic_push(S, &token) != SUCCESS) {//push EOS(lowest precedence) onto the stack to mark the bottom
        return ERROR;
    }

    token=getToken(&symbol, &index);
    while(token != EOS)
    {
        if(token == NUM)
        {
            if (!expect_operand || post_index + 1 >= post_capacity) {
                return ERROR;
            }
            post[post_index++]=symbol;
            expect_operand=0;
        }
        else if(token == LEFT_PARE)
        {
            if (!expect_operand || dynamic_push(S, &token) != SUCCESS) {
                return ERROR;
            }
        }
        else if(token == RIGHT_PARE)
        {
            if (expect_operand) {
                return ERROR;
            }
            while(dynamic_getTop(S, &e) == SUCCESS && e != LEFT_PARE && e != EOS)
            {
                if (post_index + 1 >= post_capacity || dynamic_pop(S, &e) != SUCCESS) {
                    return ERROR;
                }
                post[post_index++]=token_to_char(e);
            }
            if (e != LEFT_PARE || dynamic_pop(S, &e) != SUCCESS) {
                return ERROR;
            }
        }
        else //operator
        {
            if (expect_operand || dynamic_getTop(S, &e) != SUCCESS) {
                return ERROR;
            }
            while(e != EOS && e != LEFT_PARE &&
                  out_stack[token] <= in_stack[e])
            {
                if (post_index + 1 >= post_capacity || dynamic_pop(S, &e) != SUCCESS) {
                    return ERROR;
                }
                post[post_index++]=token_to_char(e);
                if (dynamic_getTop(S, &e) != SUCCESS) {
                    return ERROR;
                }
            }
            if (dynamic_push(S, &token) != SUCCESS) {
                return ERROR;
            }
            expect_operand=1;
        }
        token=getToken(&symbol, &index);
    }//while token != EOS

    if (expect_operand) {
        return ERROR;
    }
    while(dynamic_getTop(S, &e) == SUCCESS && e != EOS)
    {
        if (e == LEFT_PARE || post_index + 1 >= post_capacity ||
            dynamic_pop(S, &e) != SUCCESS) {
            return ERROR;
        }
        post[post_index++]=token_to_char(e);
    }
    
    if (dynamic_getTop(S, &e) != SUCCESS || e != EOS) {
        return ERROR;
    }
    if (dynamic_pop(S, &e) != SUCCESS) {
        return ERROR;
    }
    post[post_index]='\0';
    return SUCCESS;
}//end of postfix

int main(void)
{
    int initialCapacity=100;
    char post[MAXSIZE];
    DynamicStack *S=create_dynamicStack(initialCapacity, sizeof(SymbolType));
    if (S==NULL) {
        fprintf(stderr, "Failed to create stack.\n");
        return ERROR;
    }

    if (postfix(S, post, sizeof(post)) != SUCCESS) {
        fprintf(stderr, "Invalid infix expression.\n");
        destroy_dynamicStack(S);
        return ERROR;
    }
    printf("%s\n", post);
    printf("postfix expression %s \n",post);
    destroy_dynamicStack(S);
    
    return SUCCESS;
}
