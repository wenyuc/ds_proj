#include <stdio.h>
#include <stdlib.h>

#define MAXSIZE 100
#define SUCCESS 0
#define ERROR 1

typedef char ElemType;
typedef int Status;

/* 
一个非常经典且常见的 C 语言结构体与 typedef 的混用写法。
struct node 是给编译器看的“内部真实名字”（标签），
而 Node 是给程序员用的“外部快捷别名”。
如果是链表节点这种需要包含“指向自身类型的指针”的结构体，绝对不能省略标签。
必须在内部留个名字（如 struct node）让指针指向它。

有了这个定义后，后续代码中，有两种定义变量的方式.
struct node n1;  // ✅ 合法，用内部真实名字.如果不定义别名Node，后续只能这个方式定义变量，多写几个字.
Node n2;         // ✅ 合法，用外部快捷别名（更常用）
两者配合：内部用真名，外部用外号。
*/

typedef struct node{  //node 是结构体标签(Tag).它告诉编译器:我有一个结构体类型,它的内部正式名称叫node.
   ElemType data;
   struct node *next;  //在结构体内部,要定义一个指向自己的指针.因为结构体此时还没定义完,别名Node还没生效,所以编译器认的只有标签.必须用 struct node * 来声明这个指针.
} Node;  //把前面这一大坨结构体类型，起一个别名叫做 Node，给程序员用.

/*
为什么推荐用malloc()创建头节点及链表上各节点？
链表的核心在于“动态”
链表设计的初衷，就是为了克服数组“大小固定”的缺点。如果你在栈上定义 Node head;
它确实能工作，但链表真正的价值在于节点可以随时动态创建和销毁。
用 malloc() 能让整个链表的所有节点（包括头节点）保持一致的生命周期管理方式。
通过initList(), createList()外部调用者拿到的是指针，想用多久用多久。
*/

Node* initList(void)
{
    Node *head=(Node*)malloc(sizeof(Node));  //在堆上创建Node
    if (head==NULL) return NULL;

    head->data=0;
    head->next=NULL;
    return head;
}

Node* createList(void) {
    Node *head = (Node*)malloc(sizeof(Node));
    if (head == NULL) return NULL;
    head->data = 0;
    head->next = NULL;
    return head;
}

//create a Node with initial data
//Return pointer to Node
Node* createNodeWithElem(ElemType e)
{
    Node *p=(Node*)malloc(sizeof(Node));
    p->data=e;
    p->next=NULL;
    return p;
}

// insert a node at head
Status insertHead(Node* L, ElemType e)
{
    Node *p=(Node*)malloc(sizeof(Node));
    if (p==NULL) return ERROR;

    p->data=e;
    p->next=L->next;
    L->next=p;
    return SUCCESS;
}

// traverse a list from head
Status traverseList(Node* L)
{
    Node *p=L->next;
    while(p != NULL)
    {
        printf("%c\t",p->data);
        p=p->next;
    }
    printf("\n");
    return SUCCESS;
}

// get the tail node
Node* get_tail(Node* L)
{
    Node *p=L;
    while (p->next != NULL)
    {
        p=p->next;
    }
    return p;
}

//insert at tail 
Status insertTail1(Node *L, ElemType e)
{
    Node *p = (Node*)malloc(sizeof(Node));
    p->data=e;
    p->next=NULL;

    Node* tail=get_tail(L);  //get the tail Node of L
    tail->next=p;

    return SUCCESS;
}

Node* insertTail2(Node *tail, ElemType e)
{
    Node *p=(Node*)malloc(sizeof(Node));
    p->data=e;
    p->next=NULL;
    tail->next=p;
    return p;
}

//insert a node at specific position of a linked list
//return SUCCESS or ERROR
Status insertNode(Node *L, int pos, ElemType e)
{
    Node *t=L; //pre-node of the new node
    int i=0;
    while (i < pos-1)
    {
        t=t->next;
        i++;
        if (t == NULL) return ERROR;
    }

    Node *p=(Node*)malloc(sizeof(Node));
    p->data=e;
    p->next=t->next;
    t->next=p;
    return SUCCESS;
}

//delete a node at position
//Return SUCCESS or ERROR
Status deleteNode(Node *L, int pos)
{
    Node *t=L; //pre-node of the deleted node
    int i=0;
    while (i < pos-1)
    {
        t=t->next;
        i++;
        if (t == NULL) return ERROR;
    }

    if (t->next==NULL) return ERROR;

    Node *p=t->next;
    t->next=p->next;
    free(p);
    return SUCCESS;
}

//get the length of a linked list, starting from 1
//return: int
int get_length(Node *L)
{
    int len=0;
    Node *t=L;
    while (t->next != NULL)
    {
        t=t->next;
        len++;
    }
    return len;
} 

//free the nodes of the linked list, but retain the head node
//Return: SUCCESS or ERROR
Status freeList(Node *L)
{
    Node *p=L->next;
    Node *q;

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
    Node *l_list=initList();    //头节点在堆上
    /*
    若用这样创建头节点:
    Node head;
    head.data=0;
    head.next=NULL;   
    则头节点在main的栈上
    */
    if (l_list==NULL)
    {
        return ERROR;
    }
    
    ret=insertHead(l_list, 'Q');
    ret=insertHead(l_list, 'J');

    if (ret==SUCCESS) 
    {
        printf("l_list at %p l_list->next at %p l_list->next->next at %p\n",\
         (void *)l_list, (void *)l_list->next, (void *)l_list->next->next);
        
        // printf("node 1 data %c node 2 data %c node 3 data %c\n",
        // (int)l_list->data, (int)l_list->next->data, (int)l_list->next->next->data);
        ret=traverseList(l_list);
    }

    //尾插法方式1
    ret=insertTail1(l_list, 'K');
    if (ret==SUCCESS)
    {
        traverseList(l_list);
    }

    //尾插法方式2
    Node *tail=get_tail(l_list);
    tail=insertTail2(tail,'A');
    tail=insertTail2(tail,'2');

    if (tail != NULL)
    {
        traverseList(l_list);
    }
    
    //insert a node at position 2 (start from 1)
    ret=insertNode(l_list, 2, 'B');
    if (ret==SUCCESS)
    {
        traverseList(l_list);
    }

    //delete a node at position 2 (start from 1)
    ret=deleteNode(l_list, 2);
    if (ret==SUCCESS)
    {
        traverseList(l_list);
    }

    ret=deleteNode(l_list, 5);
    if (ret==SUCCESS)
    {
        traverseList(l_list);
    }

    printf("Length of l_list: %d\n", get_length(l_list));

    //free the list
    ret=freeList(l_list);
    if (ret==SUCCESS)
    {
        printf("Length of l_list: %d\n", get_length(l_list));
    }
    printf("Head is %d\n", !(l_list==NULL));

    return SUCCESS;
}
