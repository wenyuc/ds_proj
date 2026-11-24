#ifndef DS_TYPE_H
#define DS_TYPE_H

#include <stdio.h>

#define MAXSIZE 100
#define MAXVEX 100
#define MAXEDGE (MAXVEX * MAXVEX)
#define SUCCESS 0
#define ERROR 1

typedef int Status;
typedef int VertexType;//vertex type in graph
typedef int EdgeType;//edge type in graph
typedef char ElemType;
typedef enum { FALSE, TRUE } Bool;
typedef enum { HEAD, TAIL} END; //List endpoint

typedef enum {
    LEFT_PARE, RIGHT_PARE,
    ADD, SUB, MUL, DIV,MOD,
    EOS, NUM
}SymbolType;


static int g_fail = 0; //fail testcase count

//helper function for unit test
#define CHECK(cond, msg) do { \
    if (!(cond)) { printf("FAIL: %s\n", msg); g_fail++; } \
    else { printf("PASS: %s\n", msg); } \
} while (0)

#endif // DS_TYPE_H
