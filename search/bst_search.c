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
    
    // 创建二叉搜索树
    ret = createBST(&root);
    if(ret == SUCCESS)
    {
        printf("二叉搜索树创建成功\n");
        preOrder(root);  // 先序遍历打印二叉搜索树,调试用
    }
    else
        printf("二叉搜索树创建失败\n");
    
    BSTPtr searchT;  // 用于存储搜索结果的指针
    ret = search_bst(root, 53, NULL, &searchT);  // 搜索关键字为53的节点
    if(searchT)
        printf("找到元素53: %d\n", searchT->data);
    else
        printf("未找到元素53\n");
    
    ret = search_bst(root, 99, NULL, &searchT);  // 搜索关键字为99的节点
    if(searchT)
        printf("找到元素99: %d\n", searchT->data);
    else
        printf("未找到元素99\n");

    return SUCCESS;
}

