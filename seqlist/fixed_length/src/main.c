#include <stdio.h>
#include "seqlist.h"

int main(void)
{
    int ret;  //the status of a function.
    //Declare and initialize a seqlist
    SeqList list;
    initList(&list);
    printf("initialize list success, length=%d\n", list.length);
    printf("allicated memory size: %zu bytes.\n", sizeof(list.data));
    
    //append elements;
    ret=appendElem(&list, 'A');
    ret=appendElem(&list, 'B');
    ret=appendElem(&list, 'C');
    printf("append elements success, length=%d\n", list.length);
    
    //traverse elements of a list
    ret=traverseList(&list);

    //insert an element to a list
    ret=insertElement(&list, 2, 'X');
    if (ret==SUCCESS) {
        traverseList(&list);
    } else {
        return ERROR;
    }

    //delete an element from a list
    ElemType deleted_element;
    ret=deleteElement(&list, 2, &deleted_element);
    if (ret==SUCCESS) {
        printf("deleted element: %c\n", deleted_element);
        traverseList(&list);
    } else {
        return ERROR;
    }

    //find an element in a list
    ElemType find_element='B';
    int pos;
    ret=findElement(&list, find_element, &pos);
    if (ret==SUCCESS) {
        printf("element %c found at position %d (starting from 1)\n", find_element, pos);
    } else {
        printf("element %c not found in the list\n", find_element);
    }

    if (!ret)
      return SUCCESS;
    else 
      return ERROR;
}