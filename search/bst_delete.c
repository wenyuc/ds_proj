#include <stdio.h>
#include <stdlib.h>

#define SUCCESS 0
#define FAILURE 1
typedef int Status;
typedef int ElemType;  // 定义元素类型
//定义二叉树结点结构体
typedef struct node
{
    ElemType data;
    struct node *lchild;
    struct node *rchild;
}TreeNode;  //二叉树结点

typedef TreeNode* BSTPtr;  // 定义二叉搜索树类型

//先序遍历打印二叉搜索树
void preOrder(BSTPtr T)
{
    if(T == NULL)
        return;
    printf("%d ", T->data);
    preOrder(T->lchild);
    preOrder(T->rchild);
}

//在二叉搜索树中搜索元素
// T: 当前搜索子树的根节点
// key: 要搜索的元素
// parent: 当前搜索节点的父节点
// pos: 搜索结果的指针，如果找到则指向该节点，否则为NULL
// 返回值: SUCCESS表示找到，FAILURE表示未找到
Status search_bst(BSTPtr T, ElemType key, BSTPtr parent, BSTPtr *pos)
{
    if(T == NULL)
    {
        *pos = parent; //递归到空结点，查找失败，将父节点返回,后序可用于插入操作
        return FAILURE;
    }
    if(key == T->data)
    {
        *pos = T;
        return SUCCESS;
    }
    else if(key < T->data)
        return search_bst(T->lchild, key, T, pos);
    else
        return search_bst(T->rchild, key, T, pos);
    return FAILURE;
}

//插入BST
Status insert_bst(BSTPtr *T, ElemType key)
{
    BSTPtr pos;//搜索结果的指针,如果找不到的话，pos指向父节点
    BSTPtr curr;  // 用于存储当前节点的指针

    //查找插入的位置
    int ret = search_bst(*T, key, NULL, &pos);
    if(ret == FAILURE)
    {
        //如果未找到，创建新结点
        curr = (BSTPtr)malloc(sizeof(TreeNode));
        curr->data = key;
        curr->lchild = NULL;
        curr->rchild = NULL;
        //将新结点插入到正确的位置
        if(pos == NULL)
            *T = curr; //如果树为空，则新结点为根节点，相当于创建一颗新的二叉搜索树
        else if(key < pos->data)
            pos->lchild = curr; //pos为父节点，新结点插入到父节点的左子树
        else
            pos->rchild = curr; //pos为父节点，新结点插入到父节点的右子树
        return SUCCESS;
    }
    else
        return FAILURE;  // 元素已存在
}

// 前向声明
Status delete_bst(BSTPtr *T, ElemType key);

//删除节点
Status delete_node(BSTPtr *T)
{
    BSTPtr temp = *T;
    if((*T)->lchild == NULL)
    {
        *T = (*T)->rchild;
        free(temp);
    }
    else if((*T)->rchild == NULL)
    {
        *T = (*T)->lchild;
        free(temp);
    }
    else
    {
        temp = (*T)->rchild;
        while(temp->lchild != NULL)
            temp = temp->lchild;
        (*T)->data = temp->data;
        delete_bst(&((*T)->rchild), temp->data);
    }
    // free(temp);  // 这里不能直接释放temp，因为temp指向的是右子树的最小节点，已经被删除了
    return SUCCESS;
}

//递归查找并删除目标节点
Status delete_bst(BSTPtr *T, ElemType key)
{
    if(*T == NULL)
    {
        return FAILURE;
    }
    else 
    {
        if ((*T)->data == key)
        {
            return delete_node(T);
        }
        else if(key < (*T)->data)
            return delete_bst(&((*T)->lchild), key);
        else
            return delete_bst(&((*T)->rchild), key);
    }
    return FAILURE;
}

int main()
{
    int ret;
    BSTPtr root=NULL;
    
    //准备插入的初始序列
    int initKeys[] = {70,55,49,30,39,53,80,75,98,95};
    int initKeysLen = sizeof(initKeys) / sizeof(initKeys[0]);
    
    for(int i = 0; i < initKeysLen; i++)
    {
        ret = insert_bst(&root, initKeys[i]);
        if(ret == FAILURE)
            printf("插入元素%d失败\n", initKeys[i]);
    }

    // 先序遍历打印二叉搜索树,调试用
    preOrder(root);
    printf("\n");
    // 删除元素49
    ret = delete_bst(&root, 49);
    if(ret == FAILURE) printf("删除元素49失败\n");
    // 先序遍历打印二叉搜索树,调试用
    preOrder(root);
    printf("\n");

    return SUCCESS;
}

