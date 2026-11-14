/*
Dynamic Linked Queue Implementation
*/

#include <stdlib.h>
#include <string.h>
#include "queue.h"

//initialize a dynamic linked queue with initial capacity and element size
Status initialize_DynamicQueue(DynamicQueue* q, size_t initialCapacity, size_t elem_size) 
{
    if (q == NULL) {
        return ERROR;
    }

    QNode* node = (QNode*)malloc(sizeof(QNode));
    if (node == NULL) {
        free(q);
        return ERROR;
    }
    node->data = (void**)malloc(initialCapacity * elem_size);
    if (node->data == NULL) {
        free(node);
        free(q);
        return ERROR;
    }
    node->elem_size = elem_size;
    node->capacity = initialCapacity;
    node->next = NULL;

    q->front = node;
    q->rear = node;
    return SUCCESS;
}//initialize_DynamicQueue

//create a dynamic linked queue with initial capacity and element size
DynamicQueue* create_DynamicQueue(size_t initialCapacity, size_t elem_size) 
{
    DynamicQueue* q = (DynamicQueue*)malloc(sizeof(DynamicQueue));
    if (q == NULL) {
        return NULL;
    }
    q->front = NULL;
    q->rear = NULL;

    if (initialize_DynamicQueue(q, initialCapacity, elem_size) != SUCCESS) {
        free(q);
        return NULL;
    }
    return q;
}//create_DynamicQueue

Bool is_DynamicQueueEmpty(DynamicQueue* q) 
{
    if (q == NULL) {
        return FALSE;
    }
    return (q->front == q->rear);
}//is_DynamicQueueEmpty

Status enqueue_DynamicQueue(DynamicQueue* q, const void* item) 
{
    if (q == NULL || item == NULL) {
        return ERROR;
    }

    QNode* newnode = (QNode*)malloc(sizeof(QNode));
    if (newnode == NULL) {
        return ERROR;
    }
    newnode->data = (void**)malloc(q->rear->capacity * q->rear->elem_size);
    if (newnode->data == NULL) {
        free(newnode);
        return ERROR;
    }
    newnode->elem_size = q->rear->elem_size;
    newnode->capacity = q->rear->capacity;
    memcpy((char*)newnode->data + (size_t)0 * newnode->elem_size, (char*)item, newnode->elem_size);
    newnode->next = NULL;

    q->rear->next = newnode;
    q->rear = newnode;
    return SUCCESS;
}//enqueue_DynamicQueue

Status dequeue_DynamicQueue(DynamicQueue* q, void* item) 
{
    if (q == NULL || item == NULL) {
        return ERROR;
    }

    if (is_DynamicQueueEmpty(q)) {
        return ERROR;
    }

    QNode* oldnode = q->front;
    memcpy(item, (char*)oldnode->data + (size_t)0 * oldnode->elem_size, oldnode->elem_size);
    q->front = oldnode->next;
    free(oldnode->data);
    free(oldnode);
    return SUCCESS;
}//dequeue_DynamicQueue

Status peek_DynamicQueueFront(DynamicQueue* q, void* item) 
{
    if (q == NULL || item == NULL) {
        return ERROR;
    }

    if (is_DynamicQueueEmpty(q)) {
        return ERROR;
    }

    QNode* frontnode = q->front;
    memcpy(item, (char*)frontnode->data + (size_t)0 * frontnode->elem_size, frontnode->elem_size);
    return SUCCESS;
}//peek_DynamicQueueFront

Status peek_DynamicQueueRear(DynamicQueue* q, void* item) 
{
    if (q == NULL || item == NULL) {
        return ERROR;
    }

    if (is_DynamicQueueEmpty(q)) {
        return ERROR;
    }

    QNode* rearnode = q->rear;
    memcpy(item, (char*)rearnode->data + (size_t)0 * rearnode->elem_size, rearnode->elem_size);
    return SUCCESS;
}//peek_DynamicQueueRear

size_t get_DynamicQueueLength(DynamicQueue* q)
{
    if (q == NULL) {
        return 0;
    }
    size_t length = 0;
    QNode* current = q->front;
    while (current != NULL) {
        length++;
        current = current->next;
    }
    return length;
}//get_DynamicQueueLength

Status destroy_DynamicQueue(DynamicQueue* q) 
{
    if (q == NULL) {
        return ERROR;
    }
    QNode* current = q->front;
    while (current != NULL) {
        QNode* next = current->next;
        free(current->data);
        free(current);
        current = next;
    }
    free(q);
    return SUCCESS;
}//destroy_DynamicQueue