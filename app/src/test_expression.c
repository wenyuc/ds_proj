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

int eval(DynamicStack *S, int *result)
{
    char symbol;
    int op1,op2;
    int index=0;
    int value;
    SymbolType token;
    if (S == NULL || result == NULL) {
        return ERROR;
    }
    token=getToken(&symbol, &index);
    while(token != EOS)
    {
        if (token==NUM)
        {
            if (symbol < '0' || symbol > '9') {
                return ERROR;
            }
            value=symbol-'0';
            if (dynamic_push(S, &value) != SUCCESS) {
                return ERROR;
            }
        } else{
            if (token < ADD || token > MOD || dynamic_stackSize(S) < 2 ||
                dynamic_pop(S, &op2) != SUCCESS ||
                dynamic_pop(S, &op1) != SUCCESS) {
                return ERROR;
            }
            
            switch(token)
            {
                case ADD:
                    value=op1 + op2;
                    break;
                case SUB:
                    value=op1 - op2;
                    break;
                case MUL:
                    value=op1 * op2;
                    break;
                case DIV:
                    if (op2 == 0) {
                        return ERROR;
                    }
                    value=op1 / op2;
                    break;
                case MOD:
                    if (op2 == 0) {
                        return ERROR;
                    }
                    value=op1 % op2;
                    break;
                default:
                    return ERROR;
            }//switch
            if (dynamic_push(S, &value) != SUCCESS) {
                return ERROR;
            }
        }//if-else
        token=getToken(&symbol, &index);
    }//while
    if (dynamic_stackSize(S) != 1 || dynamic_pop(S, result) != SUCCESS) {
        return ERROR;
    }
    return SUCCESS;
}//eval

int main(void)
{
    int initialCapacity=20;
    DynamicStack *S=create_dynamicStack(initialCapacity, sizeof(int));
    if (S==NULL) {
        fprintf(stderr, "Failed to create stack.\n");
        return ERROR;
    }
    int result;
    if (eval(S, &result) != SUCCESS) {
        fprintf(stderr, "Invalid postfix expression.\n");
        destroy_dynamicStack(S);
        return ERROR;
    }
    printf("expression %s result=%d\n",expr, result);
    destroy_dynamicStack(S);
    return SUCCESS;
}
