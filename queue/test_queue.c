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
    ElemType data[MAXSIZE];
    int front;  //exit at front position
    int rear; //insert at rear position. In general queue, rear in range 0...MAXSIZE
}Queue;

Status initQueue(Queue *Q)
{
    Q->front=0;
    Q->rear=0;
    return SUCCESS;
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
    int ret;
    Queue Q;

    ret=initQueue(&Q);
    if (ret==SUCCESS)
    {
        equeue(&Q,'A');
        equeue(&Q,'B');
        equeue(&Q,'C');
        equeue(&Q,'D');
        equeue(&Q,'E');

        printf("Queue Length: %d\n", getLength(&Q));
        int head,tail;
        getPosition(&Q,&head,&tail);
        printf("Queue head: %d, tail: %d\n", head,tail);

        ElemType e;    //e是普通变量，不是指针， NULL是指针(void *)
        dequeue(&Q,&e);
        /*
            或者这样声明和调用
            ElemType e;
            ElemType *p=&e;  //指向合法的栈内存
            dequeue(&Q,p)
        */
        printf("DeQueue: %c\n",e);
        dequeue(&Q,&e);
        printf("DeQueue: %c\n",e);
        getPosition(&Q,&head,&tail);
        printf("Queue head: %d, tail: %d\n", head,tail);
        
    } else {
        return ERROR;
    }

}

