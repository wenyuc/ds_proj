#include <stdio.h>
#include "seqlist.h"

//风格A: 初始化函数. 传地址进来，堆上初始化数据区（推荐）

Status initList(SeqList *L)
{
    if (L == NULL) {
        return ERROR;
    }
    L->length = 0;
    return SUCCESS;    
}
