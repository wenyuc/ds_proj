/*
Queue and Linked List data structures and their operations.
*/
#ifndef QUEUE_H
#define QUEUE_H

#include <stddef.h>
#include "ds_type.h"

//Static Queue Structure
typedef struct {
    ElemType data[MAXSIZE]; //array to store data
    int front;  //exit at front position
    int rear; //insert at rear position. In general queue, rear in range 1...MAXSIZE
}StaticQueue;

//Static Queue operations
StaticQueue* create_StaticQueue();
Status initialize_StaticQueue(StaticQueue* q);
Status enqueue_StaticQueue(StaticQueue* q, ElemType item);
Status dequeue_StaticQueue(StaticQueue* q, ElemType* item);
Bool is_StaticQueueEmpty(StaticQueue* q);
Bool is_StaticQueueFull(StaticQueue* q);
Status peek_StaticQueueFront(StaticQueue* q, ElemType* item);
Status peek_StaticQueueRear(StaticQueue* q, ElemType* item);
size_t get_StaticQueueLength(StaticQueue* q);
Status get_StaticQueuePosition(StaticQueue* q, size_t *head, size_t *tail);
Status destroy_StaticQueue(StaticQueue* q);

//Dynamic Queue Structure
typedef struct qnode{
    void **data; //array to store data of any type
    size_t elem_size; //size of each element
    size_t capacity; //capacity of the array
    struct qnode *next;
}QNode;

typedef struct {
    QNode *front;  //exit at front
    QNode *rear;   //insert at end
}DynamicQueue;

//Dynamic Queue operations
DynamicQueue* create_DynamicQueue(size_t initialCapacity, size_t elem_size);
Status initialize_DynamicQueue(DynamicQueue* q, size_t initialCapacity, size_t elem_size);
Status enqueue_DynamicQueue(DynamicQueue* q, const void* item);
Status dequeue_DynamicQueue(DynamicQueue* q, void* item);
Bool is_DynamicQueueEmpty(DynamicQueue* q);
//Bool is_DynamicQueueFull(DynamicQueue* q); //seems unnecessary
Status peek_DynamicQueueFront(DynamicQueue* q, void* item);
Status peek_DynamicQueueRear(DynamicQueue* q, void* item);
size_t get_DynamicQueueLength(DynamicQueue* q);
Status destroy_DynamicQueue(DynamicQueue* q);

// Linked List Structure
typedef struct lnode{
    ElemType data;
    struct lnode *next;
}LNode; //insert and delete can happen at any position

typedef LNode LList;
//Dynamic Linked List operations. 
//Linked List can be used as a queue but starting from 1. head is the dummy node(0).
LList* create_LinkedList();
Status initialize_LinkedList(LList* head);
Status enqueue_LinkedList(LList* head, END end, const ElemType item);
Status dequeue_LinkedList(LList* head, END end, ElemType *item);
Bool is_LinkedListEmpty(LList* head);
Status peek_LinkedList(LList* head, size_t pos, ElemType *item);
Status insert_LinkedList(LList* head, size_t pos, const ElemType item);
Status delete_LinkedList(LList* head, size_t pos, ElemType *item);
Status update_LinkedList(LList* head, size_t pos, const ElemType item);
size_t get_LinkedListLength(LList* head);
Status destroy_LinkedList(LList* head);

#endif // QUEUE_H

