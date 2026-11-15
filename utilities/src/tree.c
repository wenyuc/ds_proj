/*
Tree Operations Implementation
*/

#include <stdlib.h>
#include "tree.h"

static char preorder_str[]="ABDH#K###E##CFI###G#J##";  //前序遍历造的一颗树
static int str_idx=0;

//构建一颗树，通过前序生成的一个字符串来生成二叉树结构
Status create_BiTree(BiTree *T)    //TreeNode **T;
{
    ElemType ch;
    ch=preorder_str[str_idx++];
    if (ch=='#')
    {
        *T=NULL;    //*TreeNode=NULL
    } else {
        *T=(BiTree)malloc(sizeof(TreeNode));
        if (*T==NULL) return ERROR;
        (*T)->data=ch;
        create_BiTree(&(*T)->lchild);
        create_BiTree(&(*T)->rchild);
    }
    return SUCCESS;
}//create_BiTree

Status destroy_BiTree(BiTree T)
{
    if (T)
    {
        destroy_BiTree(T->lchild);
        destroy_BiTree(T->rchild);
        free(T);
    }
    return SUCCESS;
}//destroy_BiTree


