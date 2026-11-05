#include <stdio.h>
#include <stdlib.h>

#define SUCCESS 0
#define ERROR 1

typedef char ElemType;
typedef int Status;

typedef struct binode{
   ElemType data;
   struct binode *prev, *next;
} BiNode;

BiNode* initList(void)
{
    BiNode *head=(BiNode*)malloc(sizeof(BiNode));  //在堆上创建BiNode
    if (head==NULL) return NULL;

    head->data=0;
    head->next=NULL;
    head->prev=NULL;
    return head;
}

BiNode* createList(void) {
    BiNode *head = (BiNode*)malloc(sizeof(BiNode));
    if (head == NULL) return NULL;
    head->data = 0;
    head->next = NULL;
    head->prev=NULL;
    return head;
}

//create a BiNode with initial data
//Return pointer to BiNode
BiNode* createBiNodeWithElem(ElemType e)
{
    BiNode *p=(BiNode*)malloc(sizeof(BiNode));
    p->data=e;
    p->next=NULL;
    p->prev=NULL;
    return p;
}

// insert a BiNode at head (L pointer)
Status insertHead(BiNode* L, ElemType e)
{
    BiNode *p=(BiNode*)malloc(sizeof(BiNode));
    if (p==NULL) return ERROR;

    p->data=e;
    p->next=L->next;
    p->prev=L;
    if (L->next != NULL)
    {
        L->next->prev=p;
    }
    L->next=p;
    return SUCCESS;
}

// traverse a list from head
Status traverseList(BiNode* L)
{
    BiNode *p=L->next;
    while(p != NULL)
    {
        printf("%c\t",p->data);
        p=p->next;
    }
    printf("\n");
    return SUCCESS;
}

// get the tail BiNode
BiNode* get_tail(BiNode* L)
{
    BiNode *p=L;
    while (p->next != NULL)
    {
        p=p->next;
    }
    return p;
}

//insert at tail 
//Parameter: L head of the Bi-Linked-List, BiNode*
//Parameter: e Element
//Return: SUCCESS or ERROR
Status insertTail1(BiNode *L, ElemType e)
{
    BiNode *p = (BiNode*)malloc(sizeof(BiNode));
    p->data=e;
    p->next=NULL;

    BiNode* tail=get_tail(L);  //get the tail BiNode of L
    tail->next=p;
    p->prev=tail;
    return SUCCESS;
}

//insert at tail 
//Parameter: tail node of the Bi-Linked-List, BiNode*
//Parameter: e Element
//Return: new tail node
BiNode* insertTail2(BiNode *tail, ElemType e)
{
    BiNode *p=(BiNode*)malloc(sizeof(BiNode));
    p->data=e;
    p->next=NULL;
    tail->next=p;
    p->prev=tail;
    return p;
}

//insert a BiNode at specific position of a bi-linked list
//return SUCCESS or ERROR
Status insertBiNode(BiNode *L, int pos, ElemType e)
{
    BiNode *t=L; //pre-BiNode of the new BiNode
    int i=0;
    while (i < pos-1)
    {
        t=t->next;
        i++;
        if (t == NULL) return ERROR;
    }

    BiNode *p=(BiNode*)malloc(sizeof(BiNode));
    p->data=e;
    p->next=t->next;
    p->prev=t;
    t->next=p;
    if (t->next != NULL)
    {
        t->next->prev=p;
    }

    return SUCCESS;
}

//delete a BiNode at position
//Return SUCCESS or ERROR
Status deleteBiNode(BiNode *L, int pos)
{
    BiNode *t=L; //pre-BiNode of the deleted BiNode
    int i=0;
    while (i < pos-1)
    {
        t=t->next;
        i++;
        if (t == NULL) return ERROR;
    }

    if (t->next==NULL) return ERROR;

    BiNode *p=t->next; //p to be deleted
    t->next=p->next;
    t->next->prev=t;
    
    free(p);
    return SUCCESS;
}

//get the length of a linked list, starting from 1
//return: int
int get_length(BiNode *L)
{
    int len=0;
    BiNode *t=L;
    while (t->next != NULL)
    {
        t=t->next;
        len++;
    }
    return len;
} 

//free the BiNodes of the linked list, but retain the head BiNode
//Return: SUCCESS or ERROR
Status freeList(BiNode *L)
{
    BiNode *p=L->next;
    BiNode *q;

    while (p != NULL)
    {
        q=p->next;
        free(p);
        p=q;
    }
    L->next=NULL;
    return SUCCESS;
}

int main(void)
{
    int ret;
    BiNode *bil_list=initList();    //头节点在堆上
    
    if (bil_list==NULL)
    {
        return ERROR;
    }
    
    ret=insertHead(bil_list, 'Q');
    ret=insertHead(bil_list, 'J');
    ret=insertHead(bil_list, 'K');

    if (ret==SUCCESS) 
    {
        printf("bil_list at %p bil_list->next at %p bil_list->next->next at %p\n",\
         (void *)bil_list, (void *)bil_list->next, (void *)bil_list->next->next);
        
        ret=traverseList(bil_list);
    }

    //尾插法方式1
    ret=insertTail1(bil_list, 'M');
    if (ret==SUCCESS)
    {
        traverseList(bil_list);
    }

    //尾插法方式2
    BiNode *tail=get_tail(bil_list);
    tail=insertTail2(tail,'A');
    tail=insertTail2(tail,'2');

    if (tail != NULL)
    {
        traverseList(bil_list);
    }

    //insert a BiNode at position 2 (start from 1)
    ret=insertBiNode(bil_list, 2, 'B');
    if (ret==SUCCESS)
    {
        traverseList(bil_list);
    }

    //delete a BiNode at position 2 (start from 1)
    ret=deleteBiNode(bil_list, 2);
    if (ret==SUCCESS)
    {
        traverseList(bil_list);
    }
    
    if (0) {
    ret=deleteBiNode(bil_list, 5);
    if (ret==SUCCESS)
    {
        traverseList(bil_list);
    }

    printf("Length of bil_list: %d\n", get_length(bil_list));

    //free the list
    ret=freeList(bil_list);
    if (ret==SUCCESS)
    {
        printf("Length of bil_list: %d\n", get_length(bil_list));
    }
    printf("Head is %d\n", !(bil_list==NULL));
    }

    return SUCCESS;
}
