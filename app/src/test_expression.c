/**
 * Test expression evaluation using a dynamic stack.
 */
#include <stdio.h>
#include <stdlib.h>
#include "stack.h"
#include "ds_type.h"

const char expr[] = "82/2+56*-";//postfix expression

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
}//getToken

int eval(DynamicStack *S)
{
    char symbol;
    int op1,op2;
    int index=0;
    int tmp=0;
    SymbolType token;
    int result;
    token=getToken(&symbol, &index);
    while(token != EOS)
    {
        if (token==NUM)
        {
            tmp=symbol-'0';
            dynamic_push(S, &tmp);
        } else{
            dynamic_pop(S, &op2);
            dynamic_pop(S, &op1);
            
            switch(token)
            {
                case ADD:
                    tmp=op1 + op2;
                    dynamic_push(S, &tmp);
                    break;
                case SUB:
                    tmp=op1 - op2;
                    dynamic_push(S, &tmp);
                    break;
                case MUL:
                    tmp=op1 * op2;
                    dynamic_push(S, &tmp);
                    break;
                case DIV:
                    tmp=op1 / op2;
                    printf("%d / %d = %d\n", op1, op2, tmp);
                    dynamic_push(S, &tmp);
                    break;
                case MOD:
                    tmp=op1 % op2;
                    printf("%d %% %d = %d\n", op1, op2, tmp);
                    dynamic_push(S, &tmp);
                    break;
                default:
                    break;
            }//switch
        }//if-else
        //get the top element
        dynamic_getTop(S, &tmp);
        printf("top element: %d at %d do %d\n", tmp, S->top, token);
        token=getToken(&symbol, &index);
    }//while
    dynamic_pop(S, &result);
    return result;
}//eval

int main(void)
{
    int initialCapacity=20;
    DynamicStack *S=create_dynamicStack(initialCapacity, sizeof(int));
    if (S==NULL) return ERROR;
    int result;
    result=eval(S);
    printf("expression %s result=%d\n",expr, result);
    destroy_dynamicStack(S);
    return SUCCESS;
}
