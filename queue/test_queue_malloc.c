#include <stdio.h>
#include <stdlib.h>

#define MAXSIZE 100
#define SUCCESS 0
#define ERROR 1
#define TRUE 1
#define FALSE 0

typedef char ElemType;
typedef int Status;

typedef struct {
    ElemType *data;
    int front;  //exit at front position
    int rear; //insert at rear position
}Queue;

Queue* initQueue()
{
    Queue *Q=(Queue*)malloc(sizeof(Queue));
    if (Q == NULL) return NULL;
    Q->data=(ElemType*)malloc(sizeof(ElemType) * MAXSIZE);
    if (Q->data == NULL) return NULL;
    Q->front=0;
    Q->rear=0;
    return Q;
}

int isEmpty(Queue *Q)
{
    return (Q->front==Q->rear)?TRUE:FALSE;
}

int isFull(Queue *Q)
{
    return (Q->rear >= MAXSIZE && Q->front==0) ? TRUE : FALSE;
}

Status QueueShifting(Queue *Q)
{
    if (Q->front > 0)
    {
        int diff=Q->front;
        for(int i=Q->front;i<=Q->rear; ++i)
        {
            Q->data[i-diff]=Q->data[i];
        }
        Q->front=0;
        Q->rear=Q->rear - diff;
        return SUCCESS;
    } else {
        return ERROR; //Queue is full
    }
}

Status dequeue(Queue *Q, ElemType *e)
{
    if (isEmpty(Q)) return ERROR;

    *e=Q->data[Q->front];
    Q->front++;
    return SUCCESS;
}

Status equeue(Queue *Q, ElemType e)
{
    if (Q->rear >= MAXSIZE)
    {
        if (!QueueShifting(Q))
        {
            return ERROR;    //Queue is full
        }
    }
    Q->data[Q->rear]=e;
    Q->rear++;
    return SUCCESS;
}

Status equeue_circular(Queue *Q, ElemType e)
{
    //To be implemented
    (void)Q; //告诉编译器不要报错unused-parameter
    (void)e;

    return SUCCESS;
}

Status getHead(Queue *Q, ElemType *e)
{
    if (isEmpty(Q)) return ERROR;

    *e = Q->data[Q->front];
    return SUCCESS;
}

int getLength(Queue *Q)
{
    return (Q->rear - Q->front);
}

Status getPosition(Queue *Q, int *head, int *tail)
{
    *head=Q->front;
    *tail=Q->rear;
    return SUCCESS;
}

int main(void)
{
    Queue *Q=initQueue();
    if (Q != NULL)
    {
        equeue(Q,'A');
        equeue(Q,'B');
        equeue(Q,'C');
        equeue(Q,'D');
        equeue(Q,'E');

        printf("Queue Length: %d\n", getLength(Q));
        int head,tail;
        getPosition(Q,&head,&tail);
        printf("Queue head: %d, tail: %d\n", head,tail);

        ElemType e;
        dequeue(Q,&e);
        printf("DeQueue: %c\n",e);
        dequeue(Q,&e);
        printf("DeQueue: %c\n",e);
        getPosition(Q,&head,&tail);
        printf("Queue head: %d, tail: %d\n", head,tail);
        
    } else {
        return ERROR;
    }

}

