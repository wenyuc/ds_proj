#include <stdio.h>
#include <stdlib.h>
#include "seqlist_malloc.h"

//风格A: 初始化函数. 传地址进来，堆上初始化数据区（推荐）

Status initList(SeqList *L)
{
    //不要把栈上的指针形参用malloc覆盖，这样丢失原来的地址，导致内存泄漏。应该在堆上分配内存，然后把地址赋给形参指针。
    if (L==NULL) {
        return ERROR;
    }

    L->data=(ElemType*)malloc(sizeof(ElemType) * MAXSIZE);  //为数据区分配内存，L的地址已经由main传进来
    if (L->data==NULL) 
    {
        return ERROR;
    }
    
    L->length=0;
    
    return SUCCESS;    
}

// 风格B: 初始化函数，无参函数，全部在堆分配，返回指针
// 好用：你可以把结构体到处传，只要传这个 8 字节的 list 指针就行了，完全不拷贝内部数据。
// 甚至如果在一个函数里创建它，函数返回后，这块内存依然存在（因为它在堆上，不在栈上）。
// 风险：正因为它在堆上，C 语言没有垃圾回收机制。如果你用 createList() 创建了它，最后忘记 free()，
// 这块内存就会一直占着，直到程序结束。
// 在实际的 C 语言工程（以及严蔚敏教材的代码）中，为了节省内存拷贝和方便传递，
// “返回结构体指针” 和 “传入结构体指针进行初始化” 是最主流的两套写法。
SeqList* createList(void)
{
    //分配结构体本身（堆上）
    SeqList *L= (SeqList *)malloc(sizeof(SeqList));
    if (L==NULL) return NULL;

    //为数据区分配内存（堆上）
    L->data=(ElemType *)malloc(sizeof(ElemType) * MAXSIZE);
    if (L->data==NULL) {
        free(L); //必须把结构体本身释放掉，否则泄漏
        return NULL;
    }

    L->length=0;
    return L;
}
