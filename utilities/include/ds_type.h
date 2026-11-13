#ifndef DS_TYPE_H
#define DS_TYPE_H

#define MAXSIZE 100
#define SUCCESS 0
#define ERROR 1

typedef int Status;
typedef char ElemType;
typedef enum { FALSE, TRUE } Bool;

typedef enum {
    LEFT_PARE, RIGHT_PARE,
    ADD, SUB, MUL, DIV,MOD,
    EOS, NUM
}SymbolType;

#endif // DS_TYPE_H