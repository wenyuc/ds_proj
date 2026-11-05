#include <stdio.h>
#include <stdlib.h>

#define MAXSIZE 100
#define SUCCESS 0
#define ERROR 1

typedef char ElemType;
typedef int Status;
typedef enum {false, true}Bool;

typedef struct binode
{
    ElemType data;
    struct bide *lchild;
    struct binode *rchild;
}BiTreeNode;  //二叉树结点

typedef struct trnode
{
    ElemType data;
    struct trnode *lchild;
    struct trnode *rchild;
    int ltag,rtag;  //ltag,rtag:0 point to child；1: point to prev or next node
}ThreadTreeNode;  //线索树结点

typedef ThreadTreeNode* ThreadTree; //Thread Tree
typedef BiTreeNode* BiTree; //Binary Tree

char str[]="ABDH#K###E##CFI###G#J##";  //前序遍历造的一颗树
int idx=0;

ThreadTree prev; //保存上一个访问的结点

//构建一颗树，通过前序生成的一个字符串来生成二叉树结构
Status createTree(ThreadTree *T)    //ThreadTreeNode **T;
{
    ElemType ch;
    ch=str[idx++];
    if (ch=='#')
    {
        *T=NULL;    //*T=NULL
    } else {
        //ThreadTree 的类型是ThreadTreeNode *
        *T=(ThreadTree)malloc(sizeof(ThreadTreeNode));
        if (*T==NULL) return ERROR;
        (*T)->data=ch;
        createTree(&(*T)->lchild);
        if ((*T)->lchild != NULL)
        {
            (*T)->ltag=0;
        }
        
        createTree(&(*T)->rchild);
        if ((*T)->rchild != NULL)
        {
            (*T)->rtag=0;
        }
    }
    return SUCCESS;
}

//具体线索化
Status threading (ThreadTree T)
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
Status inOrderThreading(ThreadTree *Head, ThreadTree T)
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
}

//TODO: 线索方式实现的前序遍历
Status traverseThreadTree_pre(void)
{
    return SUCCESS;
}

//线索方式实现的中序遍历
void traverseThreadTree_in(ThreadTree T)
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
    return;
}

//TODO:线索方式实现的中序遍历
Status traverseThreadTree_post(void)
{
    return SUCCESS;
}

int main(void)
{
    int ret;
    ThreadTree Head;
    ThreadTree T;

    ret= createTree(&T);
    if (ret==SUCCESS)
    {
        printf("Indexing the Tree ");
        inOrderThreading(&Head, T);  //不会修改T？
        printf("Traverse base on Index:\n");
        traverseThreadTree_in(Head);
    }
    
    return SUCCESS;
}
