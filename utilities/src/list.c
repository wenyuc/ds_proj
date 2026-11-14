/*
Linked List Implementation
*/

#include <stdlib.h>
#include "queue.h"

LList* create_LinkedList()
{
    LNode *head=(LNode*)malloc(sizeof(LNode));  //在堆上创建Node
    if (head==NULL) return NULL;

    if (initialize_LinkedList(head) != SUCCESS) {
        free(head);
        return NULL;
    }
    return head;
}//create_LinkedList

Status initialize_LinkedList(LList* l)
{
    if (l==NULL) return ERROR;
    l->data=0;
    l->next=NULL;
    return SUCCESS;
}//initialize_LinkedList

Bool is_LinkedListEmpty(LList* head)
{
    return head->next == NULL ? TRUE: FALSE;
}

static LNode* create_LNode(ElemType item)
{
    LNode *p=(LNode*)malloc(sizeof(LNode));
    if (p==NULL) return NULL;
    p->data=item;
    p->next=NULL;
    return p;
}//create_LNode static

//insert into linked list at head or tail
Status enqueue_LinkedList(LList* head, END end, const ElemType e)
{
    LNode *node=create_LNode(e);
    switch(end)
    {
        case HEAD:
            node->next=head->next;
            head->next=node;
            break;
        case TAIL: ;//"case *:"is a label, following a statement not declaration, so add a semicolon to avoid syntax error
            LNode *p=head;
            while(p->next!=NULL) p=p->next;
            p->next=node;
            break;
        default:
            free(node);
            return ERROR;
    }
    return SUCCESS;
}//enqueue_LinkedList

//remove from linked list at head or tail and return its data
Status dequeue_LinkedList(LList* head, END end, ElemType *item)
{
    LNode *p=head;
    switch(end)
    {
        case HEAD:
            if (head->next==NULL) return ERROR;
            p=head->next;
            head->next=p->next;
            *item=p->data;
            free(p);
            break;
        case TAIL:
            while(p->next->next!=NULL) p=p->next;
            LNode *q=p->next;
            p->next=NULL;
            *item=q->data;
            free(q);
            break;
        default:
            return ERROR;
    }
    return SUCCESS;
}//dequeue_LinkedList

//head as dummy node, pos starts from 1 as the first actual node i.e., head->next
Status peek_LinkedList(LList* head, size_t pos, ElemType* item)
{
    if (is_LinkedListEmpty(head)) return ERROR;

    LNode *p=head; //pre-node of pos
    size_t i=0;
    while(p->next!=NULL && i<pos-1)
    {
        p=p->next;
        i++;
    }
    if (i!=pos-1) return ERROR;
    *item=p->next->data;
    return SUCCESS;
}//peek_LinkedList

Status insert_LinkedList(LList* head, size_t pos, const ElemType item)
{
    LNode *p=head; //pre-node of pos
    size_t i=0;
    while(p->next!=NULL && i<pos-1)
    {
        p=p->next;
        i++;
    }
    if (i!=pos-1) return ERROR;
    LNode *node=create_LNode(item);
    if (node==NULL) return ERROR;
    node->next=p->next;
    p->next=node;
    return SUCCESS;
}//insert_LinkedList

//delete from linked list at a specific position(starting from 1) and return its data
Status delete_LinkedList(LList* head, size_t pos, ElemType *item)
{
    LNode *p=head; //pre-node of pos
    size_t i=0;
    while(p->next!=NULL && i<pos-1)
    {
        p=p->next;
        i++;
    }
    if (i!=pos-1) return ERROR;
    LNode *node=p->next;
    p->next=node->next;
    *item=node->data;
    free(node);
    return SUCCESS;
}//delete_LinkedList

Status update_LinkedList(LList* head, size_t pos, const ElemType item)
{
    LNode *p=head; //pre-node of pos
    size_t i=0;
    while(p->next!=NULL && i<pos-1)
    {
        p=p->next;
        i++;
    }
    if (i!=pos-1) return ERROR;
    p->next->data=item;
    return SUCCESS;
}//update_LinkedList

//get the length of the linked list
size_t get_LinkedListLength(LList* head)
{
    LNode *p=head->next;
    size_t count=0;
    while(p!=NULL)
    {
        p=p->next;
        count++;
    }
    return count;
}//get_LinkedListLength

//still keep head as the dummy node
Status destroy_LinkedList(LList* head)
{
    LNode *p=head->next;
    while(p!=NULL)
    {
        LNode *q=p;
        p=p->next;
        free(q);
    }
    head->next=NULL;
    return SUCCESS;
}//destroy_LinkedList