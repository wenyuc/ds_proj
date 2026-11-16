/*
Tree Operations Implementation
*/

#include <stdio.h>
#include <stdlib.h>
#include "tree.h"

static char preorder_str[]="ABDH#K###E##CFI###G#J##";  //前序遍历造的一颗树
static int str_idx=0;

//BiTree operations implementation
//构建一颗树，通过前序生成的一个字符串来生成二叉树结构
BiTree create_BiTree(void)
{    
    BiTree *T=(BiTree*)malloc(sizeof(BiTreeNode));
    if (T==NULL) return NULL;
    if (initialize_BiTree(T) != SUCCESS)
    {
        free(T);
        return NULL;
    }
    return *T;
}//create_BiTree

Status initialize_BiTree(BiTree *T)
{
    ElemType ch;
    ch=preorder_str[str_idx++];

    if (T==NULL) return ERROR;
    if (ch=='#')
    {
        *T=NULL;    //*TreeNode=NULL
    } else {
        *T=(BiTree)malloc(sizeof(BiTreeNode));
        if (*T==NULL) return ERROR;
        (*T)->data=ch;
        initialize_BiTree(&(*T)->lchild);
        initialize_BiTree(&(*T)->rchild);
    }
    return SUCCESS;
}//initialize_BiTree

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

//递归方式实现的前序遍历
Status preorder_Traversal(const BiTree T)
{
    if (T==NULL) return ERROR;

    printf("%c ", T->data);
    preorder_Traversal(T->lchild);
    preorder_Traversal(T->rchild);
    return SUCCESS;
}//preorder_Traversal

//中序遍历
Status inorder_Traversal(const BiTree T)
{
    if (T==NULL) return ERROR;

    inorder_Traversal(T->lchild);
    printf("%c ", T->data);
    inorder_Traversal(T->rchild);
    return SUCCESS;
}//inorder_Traversal

//后序遍历
Status postorder_Traversal(const BiTree T)
{
    if (T==NULL) return ERROR;

    postorder_Traversal(T->lchild);
    postorder_Traversal(T->rchild);
    printf("%c ", T->data);
    return SUCCESS;
}//postorder_Traversal

//ThreadTree operations implementation
static ThreadTree prev; //保存上一个访问的结点

//create a ThreadTree
ThreadTree create_ThreadTree(void)
{
    ThreadTree *T=(ThreadTree*)malloc(sizeof(ThreadTreeNode));
    if (T==NULL) return NULL;
    if (initialize_ThreadTree(T) != SUCCESS)
    {
        free(T);
        return NULL;
    }
    return *T;
}

//initialize ThreadTree with specific pre-order string
Status initialize_ThreadTree(ThreadTree *T)
{
    ElemType ch;
    ch=preorder_str[str_idx++];
    if (ch=='#')
    {
        *T=NULL;    //*T=NULL
    } else {
        //ThreadTree 的类型是ThreadTreeNode *
        *T=(ThreadTree)malloc(sizeof(ThreadTreeNode));
        if (*T==NULL) return ERROR;
        (*T)->data=ch;
        initialize_ThreadTree(&(*T)->lchild);
        if ((*T)->lchild != NULL)
        {
            (*T)->ltag=0;
        }
        
        initialize_ThreadTree(&(*T)->rchild);
        if ((*T)->rchild != NULL)
        {
            (*T)->rtag=0;
        }
    }
    return SUCCESS;
}

//具体线索化实现
static Status threading (ThreadTree T)
{
    if (T != NULL)
    {
        threading(T->lchild);
        if(T->lchild == NULL)
        {
            T->ltag=1;
            T->lchild=prev;
        }
        if (prev->rchild == NULL)
        {
            prev->rtag=1;
            prev->rchild=T;
        }
        prev=T;
        threading(T->rchild);
    }
    return SUCCESS;
}

//开始线索化,创建头结点，处理头结点，调用具体线索化
Status inorder_Threading(ThreadTree *Head, ThreadTree T)
{
    //生成头结点
    *Head = (ThreadTree)malloc(sizeof(ThreadTreeNode));
    if (*Head==NULL) return ERROR;

    (*Head)->ltag=0;
    (*Head)->rtag=1;
    (*Head)->rchild=(*Head);   //之后指向最后一个结点

    if (T==NULL)
    {
        (*Head)->lchild=*Head;
    } else {
        (*Head)->lchild=T;
        prev=(*Head);

        threading(T);  //每一个结点的空位加前驱加后继

        //遍历的最后一个结点线索化
        prev->rchild = *Head;
        prev->rtag = 1;

        //头结点的右孩子指向最后一个结点
        (*Head)->rchild=prev;
    }
    return SUCCESS;
}//inorder_Threading

Status inorder_Traversal_ThreadTree(const ThreadTree T)
{
    ThreadTree curr;
    curr=T->lchild;

    while(curr != T)
    {
        while(curr->ltag == 0)
        {
            curr=curr->lchild;
        }
        
        printf("%c ",curr->data);

        while(curr->rtag == 1 && curr->rchild != T)//通过线索找后继
        {
            curr=curr->rchild;
            printf("%c ",curr->data);
        }
        curr=curr->rchild;
    }
    printf("\n"); //输出结束
    return SUCCESS;
}//inorder_Traversal_ThreadTree
