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

//用数组模拟先序创建一颗二叉树（-1表示空结点）
int treeArr[]={78,55,49,30,-1,39,-1,-1,53,-1,-1,-1,80,75,-1,-1,98,95,-1,-1,-1};
int treeArrLen = sizeof(treeArr) / sizeof(treeArr[0]);  // 数组长度
int idx=0;

//创建二叉树
Status createBST(BSTPtr *T) //等价于TreeNode **
{
    ElemType num;
    num = treeArr[idx++];  //取当前索引位置的值
    if(num == -1)
    {
        *T = NULL;
        return SUCCESS;
    }
    *T = (BSTPtr)malloc(sizeof(TreeNode));
    (*T)->data = num;
    createBST(&((*T)->lchild));
    createBST(&((*T)->rchild));
    return SUCCESS;
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

//先序遍历打印二叉搜索树
void preOrder(BSTPtr T)
{
    if(T == NULL)
        return;
    printf("%d ", T->data);
    preOrder(T->lchild);
    preOrder(T->rchild);
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
    
    insert_bst(&root, 99);
    preOrder(root);  // 先序遍历打印二叉搜索树,调试用
    printf("\n");
    return SUCCESS;
}

