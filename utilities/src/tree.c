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
    BiTree *T=(BiTree*)malloc(sizeof(BiTree));//T在堆上，假设T的地址为0x2000，此时地址0x2000起8个字节的内容为NULL，T指向BiTree变量.
    if (T==NULL) return NULL;
    str_idx=0;
    if (initialize_BiTree(T) != SUCCESS) //initialize_BiTree()后，T=0x2000, *T=0x1000
    {
        free(T);
        return NULL;
    }
    /*
    如果字节return *T， 将0x1000返回（即将根结点地址返回）没问题，但是T所在的地址0x2000就没有指针指向它，会造成内存泄露
    要用如下方式返回
    */
    BiTree root=*T;//先把根地址存起来
    free(T);//再释放根这块堆内存
    return root;//返回根地址
}//create_BiTree

Status initialize_BiTree(BiTree *T)
{
    if (T == NULL) return ERROR;

    if (preorder_str[str_idx] == '\0') return ERROR;   /* 串意外结束 */
    ElemType ch = preorder_str[str_idx++];

    if (ch == '#') {
        *T = NULL;
        return SUCCESS;
    }

    *T = (BiTree)malloc(sizeof(BiTreeNode));//A结点{data='A',lchild=..., rchild=...}<-根结点. 假设它的地址为0x1000（malloc）分配，然后赋值给*T, 地址0x2000的内容为0x1000
    if (*T == NULL) return ERROR;

    (*T)->data = ch;

    if (initialize_BiTree(&(*T)->lchild) != SUCCESS) {
        destroy_BiTree(*T);// 释放 *T 及已建左子树 
        return ERROR;
    }
    if (initialize_BiTree(&(*T)->rchild) != SUCCESS) {
        destroy_BiTree(*T);
        return ERROR;
    }
    return SUCCESS;
}//initialize_BiTree

Status destroy_BiTree(BiTree T)
{
    if (T==NULL) return ERROR;
    destroy_BiTree(T->lchild);
    destroy_BiTree(T->rchild);
    free(T);
    T = NULL;
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

Status levelorder_Traversal(const BiTree T, BiTree queue[MAXSIZE])
{
    if (T==NULL) return ERROR;

    //BiTree queue[MAXSIZE];
    int front=0;
    int rear=0;
    queue[rear]=T;   //enqueue at rear
    rear=rear+1;

    while(rear != front) //queue not empty
    {
        BiTree curr=queue[front];  //dequeue from front
        front++;
        printf("%c ", curr->data);
        if (curr->lchild != NULL)
        {
            queue[rear]= curr->lchild;
            rear++;
        }
        if (curr->rchild != NULL)
        {
            queue[rear]= curr->rchild;
            rear++;
        }
    }
    return SUCCESS; 
}//levelorder_Traversal

size_t get_BiTree_Depth1(const BiTree T)
{
    if (T==NULL) return 0;

    size_t left_depth=get_BiTree_Depth1(T->lchild);
    size_t right_depth=get_BiTree_Depth1(T->rchild);

    return (left_depth > right_depth ? left_depth : right_depth) + 1;    
}//get_BiTree_Depth1

Status get_BiTree_Depth(const BiTree T, size_t *depth, size_t *node_count)
{ 
    if (T==NULL)
    {
        *depth=0;
        *node_count=0;
        return SUCCESS;
    }

    *depth=0;
    *node_count=0;
    BiTree queue[MAXSIZE];
    int front=0;
    int rear=0;
    queue[rear]=T;//enqueue at rear
    rear=rear+1;

    while(front != rear) //queue not empty
    {
        int count=rear-front;
        (*node_count)+=count;
        while (count >0 )
        {
            BiTree curr=queue[front];//dequeue at front
            front++;
            if (curr->lchild != NULL)
            {
                queue[rear]= curr->lchild;
                rear++;
            }
            if (curr->rchild != NULL)
            {
                queue[rear]= curr->rchild;
                rear++;
            }
            count--;
        }//while count
        (*depth)++;
    }//while front != rear

    return SUCCESS;
}//get_BiTree_Depth


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
    ThreadTree *T=(ThreadTree*)malloc(sizeof(ThreadTree));
    if (T==NULL) return NULL;
    str_idx=0;
    if (initialize_ThreadTree(T) != SUCCESS)
    {
        free(T);
        return NULL;
    }
    ThreadTree root=*T;
    free(T);
    return root;
}//create_ThreadTree

//initialize ThreadTree with specific pre-order string
Status initialize_ThreadTree(ThreadTree *T)
{
    if (T==NULL) return ERROR;
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
        (*T)->ltag=0;
        (*T)->rtag=0;
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
}//initial_ThreadTree

static Status destroy_thread_nodes(ThreadTree T)
{
    if (T==NULL) return ERROR;
    if (T->ltag==0 && T->lchild!=T)
    {
        destroy_thread_nodes(T->lchild);
    }
    if (T->rtag==0 && T->rchild!=T)
    {
        destroy_thread_nodes(T->rchild);
    }
    free(T);
    T = NULL;
    return SUCCESS;
}//destroy_thread_nodes

Status destroy_ThreadTree(ThreadTree T)
{
    if (T==NULL) return ERROR;
    if ( destroy_thread_nodes(T)==SUCCESS)
    {
        return SUCCESS;
    } else {
        return ERROR;
    }
}//destroy_ThreadTree

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
}//threading

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
