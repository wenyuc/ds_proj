#include <stdio.h>
#include <stdlib.h>

#define SUCCESS 0
#define ERROR 1
#define TRUE 1
#define FALSE 0

typedef char ElemType;
typedef int Status;

typedef struct qnode{
    ElemType data;
    struct qnode *next;
}QNode;

typedef struct {
    QNode *front;  //exit at front
    QNode *rear;   //insert at end
}Queue;

/*
本例子中，队头元素（initQueue中的node指针）为队首，它的next指向队列中第一个节点
在链式队列中 Queue->front一直指向队首，队列中第一个节点为Q->front->next.
Queue->rear指向队列中最后一个节点。
注意实现中入队和出队的位置.
*/
Queue* initQueue()
{
    Queue *Q=(Queue*)malloc(sizeof(Queue));
    if(Q==NULL) return NULL;

    QNode *node=(QNode*)malloc(sizeof(QNode));
    if (node==NULL)
    {
        free(Q);
        return NULL;
    }

    node->data=' ';
    node->next=NULL;
    Q->front=node;
    Q->rear=node;

    return Q;
}

int isEmpty(Queue *Q)
{
    return (Q->front==Q->rear) ? TRUE : FALSE;
}

//en-queue
//insert at the end node of the liked queue.
Status equeue(Queue *Q, ElemType e)
{
    QNode *node=(QNode*)malloc(sizeof(QNode));
    if (node==NULL) return ERROR;

    node->data=e;
    node->next=NULL;
    Q->rear->next=node;
    Q->rear=node;
    return SUCCESS;
}

Status dequeue(Queue *Q, ElemType *e)
{
    QNode *node=Q->front->next;
    *e=node->data;
    Q->front->next=node->next;
    if(Q->rear==node)
    {
        Q->rear=Q->front;
    }
    free(node);
    return SUCCESS;
}

Status getFront(Queue *Q, ElemType *e)
{
    if(isEmpty(Q)) return ERROR;

    *e=Q->front->next->data;
    return SUCCESS;
}

int main(void)
{
    Queue *Q=initQueue();
    if (Q==NULL) return ERROR;

    equeue(Q, 'L');
    equeue(Q, 'M');
    equeue(Q, 'N');

    ElemType e;
    dequeue(Q, &e);
    printf("de-queue: %c\n", e);
    dequeue(Q, &e);
    printf("de-queue: %c\n", e);
    getFront(Q,&e);
    printf("First Node data: %c\n",e);

    return SUCCESS;


}