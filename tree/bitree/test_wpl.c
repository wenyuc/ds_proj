#include <stdio.h>
#include <stdlib.h>

#define MAXSIZE 100
#define SUCCESS 0
#define ERROR 1

typedef int Status;
typedef int DataType;
typedef enum {false, true} Bool;

typedef struct node
{
    DataType weight;
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

int weight[]={100, 42,15,-1,-1,27,-1,-1,58,28,13,5,-1,-1,8,-1,-1,15,-1,-1, 30,-1,-1}; //用于前序方式构造二叉树
int idx=0;

//通过前序遍历生成的字符串生成一个二叉树
Status createTree(BiTree *T)
{
    DataType d;
    d=weight[idx++];
    if (d == -1)
    {
        *T=NULL;    //*TreeNode=NULL
        return SUCCESS;
    } else {
        *T=(BiTree)malloc(sizeof(TreeNode));
        if (*T==NULL) return ERROR;
        (*T)->weight=d;
        createTree(&(*T)->lchild);
        createTree(&(*T)->rchild);
    }
    return SUCCESS;
}

//创建并初始化一个队列，队列中的结点是树结点
Status initQueue(Queue **Q)
{
    *Q=(Queue*)malloc(sizeof(Queue));
    if (*Q==NULL) return ERROR;
    (*Q)->data=(ElemType *)malloc(sizeof(ElemType) * MAXSIZE);
    if ((*Q)->data ==NULL)
    {
        free(Q);
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

Status wpl(BiTree root, int *depth, int *wpl)
{
    if (root==NULL) return SUCCESS;

    BiTree queue[MAXSIZE];
    int front=0;
    int rear=0;
    queue[rear]=root;   //enqueue at rear
    rear=rear+1;

    while(rear != front) //queue not empty
    {
        int count = rear - front; //queue size
        while (count > 0)
        {
            BiTree curr=queue[front];  //dequeue from front
            front++;
            if (curr->lchild ==NULL && curr->rchild == NULL)
            {
                *wpl += (*depth) * curr->weight;
            }
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
        }
        (*depth)++;
    }
    return SUCCESS;
}

int main(void)
{
    int ret;
    BiTree Tree=NULL;
    ret=createTree(&Tree);
    if (ret==SUCCESS && Tree != NULL)
    {
        int depth=0;
        int w=0;
        ret=wpl(Tree,&depth, &w);
        printf("depth:%d wpl:%d\n",depth, w);
    }

    free(Tree);
    return SUCCESS;
}