#ifndef SEQLIST_H
#define SEQLIST_H

#include <stdio.h>

#define MAXSIZE 100
#define SUCCESS 0
#define ERROR 1

typedef int Status;
typedef char ElemType;

typedef struct {
    ElemType data[MAXSIZE];
    int length;
} SeqList;

// 函数声明
Status initList(SeqList *L);
Status appendElem(SeqList *L, ElemType e);
Status traverseList(SeqList *L);
Status insertElement(SeqList *L, int pos, ElemType e);
Status getElement(SeqList *L, int pos, ElemType *e);
Status deleteElement(SeqList *L, int pos, ElemType *e);
Status findElement(SeqList *L, ElemType e, int *pos);

#endif // SEQLIST_H