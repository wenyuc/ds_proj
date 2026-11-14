/*
Static Queue Operations Implementation
*/
#include <stdlib.h>
#include "queue.h"

StaticQueue* create_StaticQueue() 
{
    StaticQueue* q = (StaticQueue*)malloc(sizeof(StaticQueue));
    if (q == NULL) {
        return NULL;
    }
    initialize_StaticQueue(q);
    return q;
}//create_StaticQueue

Status initialize_StaticQueue(StaticQueue* q) 
{
    if (q == NULL) {
        return ERROR;
    }
    q->front = 0;
    q->rear = 0;
    return SUCCESS;
}//initialize_StaticQueue

Bool is_StaticQueueEmpty(StaticQueue* q) 
{
    if (q == NULL) {
        return FALSE;
    }
    return (q->front == q->rear) ? TRUE : FALSE;
}//is_StaticQueueEmpty

Bool is_StaticQueueFull(StaticQueue* q) 
{
    if (q == NULL) {
        return FALSE;
    }
    return (q->rear - q->front == MAXSIZE) ? TRUE : FALSE;
}//is_StaticQueueFull

Status dequeue_StaticQueue(StaticQueue *q, ElemType *e)
{
    if (is_StaticQueueEmpty(q)) return ERROR;

    *e=q->data[q->front];
    q->front++;
    return SUCCESS;
}

static Status QueueShifting(StaticQueue *q)
{
    if (q->front == 0) {return SUCCESS;} //No need to shift if front is already 0
    
    int diff=q->front;
    for(int i=q->front;i<=q->rear; ++i)
    {
        q->data[i-diff]=q->data[i];
    }
    q->front=0;
    q->rear=q->rear - diff;
    return SUCCESS;   
}//shift queue

Status equeue_StaticQueue(StaticQueue *q, ElemType e)
{
    if (!is_StaticQueueFull(q) && q->rear >= MAXSIZE)
    {//no full but rear reaches the end of the array
        if (!QueueShifting(q))
        {
            return ERROR;   //Queue is full
        }
    }
    q->data[q->rear]=e;
    q->rear++;
    return SUCCESS;
}//enqueue_StaticQueue

size_t get_StaticQueueLength(StaticQueue* q)
{
    if (q == NULL) {
        return 0;
    }
    return (size_t)(q->rear - q->front);
}//get_StaticQueueLength

Status peek_StaticQueueFront(StaticQueue* q, ElemType* item)
{
    if (is_StaticQueueEmpty(q)) return ERROR;
    *item = q->data[q->front];
    return SUCCESS;
}//peek_StaticQueueFront

Status peek_StaticQueueRear(StaticQueue* q, ElemType* item)
{
    if (is_StaticQueueEmpty(q)) return ERROR;
    *item = q->data[q->rear - 1];
    return SUCCESS;
}//peek_StaticQueueRear

Status get_StaticQueuePosition(StaticQueue *q, size_t *head, size_t *tail)
{
    *head=q->front;
    *tail=q->rear;
    return SUCCESS;
} //get_StaticQueuePosition

Status destroy_StaticQueue(StaticQueue* q)
{
    if (q == NULL) {
        return ERROR;
    }
    free(q);
    return SUCCESS;
}//destroy_StaticQueue
