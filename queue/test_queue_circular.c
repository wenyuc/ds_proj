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
    int rear; //insert at rear position. In circular queue, rear in range of 0...MAXSIZE-1,每次移动都会取模运算绕回.
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
    return (((Q->rear + 1) % MAXSIZE) == Q->front) ? TRUE : FALSE;
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
    if (Q == NULL || e == NULL) return ERROR;

    if (isEmpty(Q)) return ERROR;

    *e=Q->data[Q->front];
    
    //抹除旧数据，可选，防偷窥
    Q->data[Q->front]=0;

    Q->front = (++Q->front) % MAXSIZE;
    return SUCCESS;
}

Status equeue(Queue *Q, ElemType e)
{
    /*
    假设maxsize=5, 循环队列插入4个元素后的情形：
    入队 D：front=0, rear=4 (严蔚敏教材中rear的位置是最后队尾元素的下一个元素)
    [A][B][C][D][ ]
    f            r
    这时还有一个位置，但如果插入了后，变为：
    [A][B][C][D][E]  ← 5个元素全满
    f/r （front=0, rear=0）
    此时front ==rear==0,这和空队列判断条件front==rear一摸一样，无法区分.
    为了避免“满”和“空”无法区分，我们规定：当 rear 的下一个位置就是 front 时，认为队列已满。
    这就是“牺牲一个空间”的含义：数组有 5个(MAXSIZE)格子，但最多只能存 4 个(MAXSIZE-1)元素
    */
    if (((Q->rear + 1) % MAXSIZE) == Q->front)
    {
        return ERROR; //circular queue is full.
    }
    Q->data[Q->rear]=e;
    Q->rear=( ++Q->rear) % MAXSIZE;
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

        ElemType e;
        dequeue(&Q,&e);
        
        printf("DeQueue: %c\n",e);
        dequeue(&Q,&e);
        printf("DeQueue: %c\n",e);
        getPosition(&Q,&head,&tail);
        printf("Queue head: %d, tail: %d\n", head,tail);
        
    } else {
        return ERROR;
    }

}

