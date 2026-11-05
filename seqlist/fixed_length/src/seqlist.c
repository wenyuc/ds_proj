#include "seqlist.h"

//Append Element to the end of the list
Status appendElem(SeqList *L, ElemType e)
{
    if (L==NULL) {
        return ERROR;
    }
    if (L->length>=MAXSIZE) {
        //printf("list is full, cannot append element.\n");
        return ERROR;
    }
    L->data[L->length]=e;
    L->length++;
    return SUCCESS;
}

//Traverse the list from beginning to end.
Status traverseList(SeqList *L)
{
    if (L==NULL) {
        return ERROR;
    }

    for (int i=0; i<L->length;i++){
        printf("%c\t", L->data[i]);
    }

    printf("\n");
    return SUCCESS;
}


//Insert an element to position of a list
Status insertElement(SeqList *L, int pos, ElemType e) 
{
    if (1<= pos && pos <=L->length) 
    {
        for (int i=L->length-1; i>=pos-1;i--)
        {
            L->data[i+1]=L->data[i];
        }
        L->data[pos-1]=e;
        L->length++;

    } else {
        return ERROR;
    }
    return SUCCESS;
}

//delete an element from a list
Status deleteElement(SeqList *L, int pos, ElemType *e)
{
    if (pos >= MAXSIZE)
    {
        return ERROR;
    }

    if (pos <=L->length-1) 
    {
        *e=L->data[pos-1];
        for (int i=pos-1; i<L->length-1;i++)
        {
            L->data[i]=L->data[i+1];
        }
        L->length--;
    }
    return SUCCESS;
}

//find an element in a list and return its posistion of first occurance.
Status findElement(SeqList *L, ElemType e, int *pos)
{
    if (L==NULL) {
        return ERROR;
    }

    for (int i=0; i<L->length; i++) {
        if (L->data[i] == e) {
            *pos = i+1;  //return the position of the element, starting from 1
            return SUCCESS;
        }
    }

    return ERROR;
}
