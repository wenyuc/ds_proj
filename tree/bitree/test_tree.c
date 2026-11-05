#include <stdio.h>
#include <stdlib.h>

#define MAXSIZE 100
#define SUCCESS 0
#define ERROR 1

typedef char ElemType;
typedef int Status;
typedef enum {false, true}Bool;

//用于非递归方式遍历二叉树
/*
typedef struct {
    TreeNode *tnode;
    int top;
}Stack;
*/

typedef struct node
{
    ElemType data;
    struct node *lchild;
    struct node *rchild;
}TreeNode;  //二叉树结点

typedef struct indexnode
{
    ElemType data;
    struct indexnode *lchild;
    struct indexnode *rchild;
    int ltag,rtag;  //ltag,rtag:0 point to child；1: point to prev or next node
}IndexNode;  //线索树结点

typedef IndexNode* InTree; //Index Tree
typedef TreeNode* BiTree; //Binary Tree

char str[]="ABDH#K###E##CFI###G#J##";  //前序遍历造的一颗树
int idx=0;

/*TODO
Bool isEmpty(Stack *S)
{
    return (S->top==0) ? true : false;
}
*/

//构建一颗树，通过前序生成的一个字符串来生成二叉树结构
Status createTree(BiTree *T)    //TreeNode **T;
{
    ElemType ch;
    ch=str[idx++];
    if (ch=='#')
    {
        *T=NULL;    //*TreeNode=NULL
    } else {
        *T=(BiTree)malloc(sizeof(TreeNode));
        if (*T==NULL) return ERROR;
        (*T)->data=ch;
        createTree(&(*T)->lchild);
        createTree(&(*T)->rchild);
    }
    return SUCCESS;
}

//递归方式实现的前序遍历
Status traverseTree_pre(BiTree T)
{
    if (T==NULL) return ERROR;

    printf("%c ", T->data);
    traverseTree_pre(T->lchild);
    traverseTree_pre(T->rchild);
    return SUCCESS;

}

//非递归方式实现的前序遍历
/*
TODO
Status iter_traverseTree_pre(Stack *S, BiTree T)
{
    while(T != NULL || isEmpty(S) )
    {
        while( T != NULL)
        {
            printf("%c ",T->data);
            push(S, T);
            T=T->lchild;
        }
        pop(S,&T);
        T=T->rchild;
    }
}
*/

//中序遍历
void traverseTree_in(BiTree T)
{
    if (T==NULL) return;

    traverseTree_in(T->lchild);
    printf("%c ", T->data);
    traverseTree_in(T->rchild);
}

//后序遍历
void traverseTree_post(BiTree T)
{
    if (T==NULL) return;

    traverseTree_post(T->lchild);
    traverseTree_post(T->rchild);
    printf("%c ", T->data);
}

int main(void)
{
    int ret;
    BiTree T;
    ret= createTree(&T);
    if (ret==SUCCESS)
    {
        printf("preorder: ");
        traverseTree_pre(T);
    }
    printf("\ninorder: ");
    traverseTree_in(T);
    printf("\npostorder:");
    traverseTree_post(T);
    printf("\n");
    return SUCCESS;
}
