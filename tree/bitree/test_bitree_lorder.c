#include <stdio.h>
#include <stdlib.h>

#define MAXSIZE 100
#define SUCCESS 0
#define ERROR 1

typedef int Status;
typedef char DataType;
typedef enum {false, true} Bool;

typedef struct node
{
    DataType data;
    struct node *lchild;
    struct node *rchild; 
}TreeNode;

typedef TreeNode* ElemType;

typedef struct
{
    ElemType *data;
    int front,rear;
}Queue;

typedef TreeNode* BiTree;

char str[]="ABDH#K###E##CFI###G#J##";  //用于构造二叉树
int idx=0;

//通过前序遍历生成的字符串生成一个二叉树
Status createTree(BiTree *T)
{
    DataType ch;
    ch=str[idx++];
    if (ch=='#')
    {
        *T=NULL;    //*TreeNode=NULL
        return SUCCESS;
    } else {
        *T=(BiTree)malloc(sizeof(TreeNode));
        if (*T==NULL) return ERROR;
        (*T)->data=ch;
        createTree(&(*T)->lchild);
        createTree(&(*T)->rchild);
    }
    return SUCCESS;
}

//创建并初始化一个队列，队列中的结点是树结点
Status initQueue(Queue **Q) //必须是Queue **类型才能接住传来的 &Qu
{
    *Q=(Queue*)malloc(sizeof(Queue)); //*Q就是调用者的Qu，赋值后 Qu 里存的是malloc分配的"Queue 对象的堆地址"，
    if (*Q==NULL) return ERROR;
    (*Q)->data=(ElemType *)malloc(sizeof(ElemType) * MAXSIZE); //Qu->data 类型是ElemType *，指针指向的地址上保存的是能容纳 MAXSIZE 个 ElemType 的堆数组的首元素地址
    if ((*Q)->data ==NULL)
    {
        free(*Q);  //注意，释放Q本体，即分配的Queue对象的堆地址
        return ERROR;
    } 

    (*Q)->front=0;
    (*Q)->rear=0;
    return SUCCESS;
}

Bool isEmpty(Queue *Q)
{
    return (Q->front == Q->rear) ? true: false;
}

//enter queue
Status enqueue(Queue *Q, ElemType e)
{
    if ((Q->rear+1) % MAXSIZE == Q->front)
    {
        printf("queue full\n");
        return ERROR;
    }
    Q->data[Q->rear]=e;
    Q->rear=(Q->rear+1) % MAXSIZE;  //circular queue
    return SUCCESS;
}

//exit the queue
Status dequeue(Queue *Q, ElemType *e)
{
    if (isEmpty(Q))
    {
        printf("empty queue\n");
        return ERROR;
    }
    *e=Q->data[Q->front];
    Q->front=(Q->front+1) % MAXSIZE;
    return SUCCESS;
}

//get front element
Status getHead(Queue *Q, ElemType *e)
{
    if (isEmpty(Q))
    {
        printf("empty queue\n");
        return ERROR;
    }
    *e=Q->data[Q->front];
    return SUCCESS;
}

//get the queue size
int getQueueSize(Queue *Q)
{
    if (isEmpty(Q))
    {
        return 0;
    }
    return Q->rear - Q->front;
}

int maxDepth(TreeNode* root)
{
    if (root==NULL) return 0;

    int depth=0;
    int ret;
    Queue *Qu=NULL;  //Qu所指向的Queue对象没有分配地址(现在Qu=NULL，NULL就是没有指向),但是Qu本身有确定的地址,
                     //它占用栈上的存储空间,这个空间在进入main是已经分配好了
                     //&Qu是某个栈地址(0x7ffe...),现在Qu的地址中保存的内容为NULL，下一步调用initQueue(),该函数中分配地址空间保存为Qu的地址的内容
    ret=initQueue(&Qu); //将局部变量Qu的地址传给initQueue, &Qu的类型是Queue **. &运算的规则是“取谁的地址，结果的类型就是‘指向谁的类型’”
    if (ret==ERROR)
    {
        return ERROR; //creating queue fail
    }
    enqueue(Qu, root);

    while(!isEmpty(Qu))
    {
        int count=getQueueSize(Qu);
        while (count >0 )
        {
            TreeNode* curr;
            dequeue(Qu,&curr);
            if (curr->lchild != NULL)
            {
                enqueue(Qu, curr->lchild);
            }
            if (curr->rchild != NULL)
            {
                enqueue(Qu, curr->rchild);
            }
            count--;
        }
        depth++;
    }
    free(Qu->data);
    free(Qu);
    Qu=NULL; //avoid hunging pointer

    return depth;
}

int main(void)
{
    int ret;
    BiTree Tree=NULL;
    ret=createTree(&Tree);
    if (ret==SUCCESS && Tree != NULL)
    {
        printf("%d\n",maxDepth(Tree));
    }

    free(Tree);
    return SUCCESS;
}