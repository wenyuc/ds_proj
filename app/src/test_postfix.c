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

//convert SymbolType to character to store in postfix array
char token_to_char(SymbolType e)
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
}//end of token_to_char

void print_token(SymbolType e)
{
    // e 可能是 NUM 的字符，也可能是运算符枚举值
    switch (e) {
        case ADD: printf("+"); break;
        case SUB: printf("-"); break;
        case MUL: printf("*"); break;
        case DIV: printf("/"); break;
        case MOD: printf("%%"); break;
        // LEFT_PARE/RIGHT_PARE 一般不会出现在后缀输出里
        case LEFT_PARE: printf("("); break;
        case RIGHT_PARE: printf(")"); break;
        case EOS: printf("EOS"); break;
        default:  printf("%c", e); break;   // NUM 的字符
    }
    return;
}//end of print_token

//Convert infix expression to postfix expression
int postfix(DynamicStack *S, char *post)
{
    int in_stack[]={0,19,12,12,13,13,13,0}; //precedence values for operators in the stack
    int out_stack[]={20,19,12,12,13,13,13,0}; //precedence values for operators outside the stack
    SymbolType token;
    char symbol;//保存运算符或操作数
    int index=0;//infix index
    int post_index=0;//postfix index
    SymbolType e;//保存出栈的内容

    token=EOS;
    dynamic_push(S, &token);//入栈EOS，作为栈底元素，它优先级最低    

    token=getToken(&symbol, &index);
    while(token != EOS)
    {
        if(token == NUM)
        {
            print_token(symbol); //output the operand
            post[post_index++]=symbol; //store the operand in the postfix array
        }
        else if(token == LEFT_PARE)
        {
            dynamic_push(S, &token);
            //dynamic_getTop(S, &e);print_token(e); //output the top element of the stack for debugging purposes
        }
        else if(token == RIGHT_PARE)
        {
            while(dynamic_getTop(S, &e) == SUCCESS && e != LEFT_PARE)
            {
                dynamic_pop(S, &e);
                print_token(e); //output the operator before storing it in the postfix array
                post[post_index++]=token_to_char(e); //store the operator in the postfix array
            }
            dynamic_pop(S, &e); //pop the left parenthesis
        }
        else //operator
        {
            dynamic_getTop(S, &e);
            //printf("%d precedence: %d\n", (SymbolType)e, in_stack[(SymbolType)e]);
            //printf("%d precedence: %d\n", token, out_stack[token]);
            while(out_stack[token] <= in_stack[(SymbolType)e])
            {
                dynamic_pop(S, &e);
                print_token(e); //output the operator before storing it in the postfix array
                post[post_index++]=token_to_char(e); //store the operator in the postfix array
                dynamic_getTop(S, &e);
            }
            dynamic_push(S, &token);
            //dynamic_getTop(S, &e);print_token(e); //output the top element of the stack for debugging purposes
        }
        token=getToken(&symbol, &index);
    }//while token != EOS

    while(dynamic_getTop(S, &e) == SUCCESS && e != EOS)
    {
        dynamic_pop(S, &e);
        print_token(e); //output the remaining operators
        post[post_index++]=token_to_char(e); //store the remaining operators in the postfix array
    }
    
    post[post_index]='\0'; //null-terminate the postfix array
    printf("\n");
    return SUCCESS;
}//end of postfix

int main(void)
{
    int initialCapacity=100;
    char post[MAXSIZE];
    DynamicStack *S=create_dynamicStack(initialCapacity, sizeof(SymbolType));
    if (S==NULL) return ERROR;

    //size_t n = sizeof(exprs) / sizeof(exprs[0]);   // n = 2
    postfix(S, post);
    printf("postfix expression %s \n",post);
    destroy_dynamicStack(S);
    
    return SUCCESS;
}
