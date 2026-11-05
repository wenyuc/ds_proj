#include <stdio.h>
#include <stdlib.h>
#include "seqlist_malloc.h"

int main(void)
{
    int ret;

    //Declare and initialize a seqlist
    /*
    SeqList list;声明的list是局部变量,栈上分配sizeof(SeqList)大小的内存(8+8对齐)，它有合法的地址，绝对不是空指针
    但list.data是一个真正的指针成员，此时它里面的存的值是随机垃圾值（野指针）不是NULL
    list.length也是一个垃圾值（比如32767， -745397）
    为了避免带有野指针和垃圾值，在声明时就将其清零.
    SeqList list={NULL,0}; 都是StyleA.结构在栈，数据区在堆

    若声明为全局变量（在所有函数之外），编译器会把它放在.bss段，并自动将其全部清零。
    此时list.data==NULL; list.length==0
    编译器会把它放在 .bss 段，并自动将其全部清零。
    list 这个结构体实体本身依然不是 NULL（它有一块固定的地址）。
    */
    //SeqList list;  
    //ret=initList(&list);

    SeqList *list=createList();  //Style B. 结构和数据区都在堆上

    if (list!=NULL)
    {
        printf("initialize list success, length=%d\n", list->length);
        printf("pointer size: %zu bytes.\n", sizeof(list->data));  //指针大小，8字节
        //printf("allocated memory size for data: %zu bytes.\n", sizeof(ElemType) * MAXSIZE);  //分配的内存大小，100字节
    } else {
        printf("initializtion error.\n");
        return ERROR;
    }

    //append elements;
    ret=appendElem(list, 'A');
    ret=appendElem(list, 'B');
    ret=appendElem(list, 'C');
    printf("append elements success, length=%d\n", list->length);
    
    //traverse elements of a list
    ret=traverseList(list);

    //insert an element to a list
    ret=insertElement(list, 2, 'X');
    if (ret==SUCCESS) {
        traverseList(list);
    } else {
        return ERROR;
    }

    //delete an element from a list
    ElemType deleted_element;
    ret=deleteElement(list, 2, &deleted_element);
    if (ret==SUCCESS) {
        printf("deleted element: %c\n", deleted_element);
        traverseList(list);
    } else {
        return ERROR;
    }

    //find an element in a list
    ElemType find_element='B';
    int pos;
    ret=findElement(list, find_element, &pos);
    if (ret==SUCCESS) {
        printf("element %c found at position %d (starting from 1)\n", find_element, pos);
    } else {
        printf("element %c not found in the list\n", find_element);
    }

    free(list->data);  //malloc --> free
    list->data=NULL;  //avoid hanging pointer

    if (!ret)
      return SUCCESS;
    else 
      return ERROR;
}