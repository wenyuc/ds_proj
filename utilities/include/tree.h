/*
Tree Implementation
*/
#ifndef TREE_H
#define TREE_H

#include "ds_type.h"

typedef struct node
{
    ElemType data;
    struct node *lchild;
    struct node *rchild;
}BiTreeNode;  //二叉树结点

typedef struct indexnode
{
    ElemType data;
    struct indexnode *lchild;
    struct indexnode *rchild;
    int ltag,rtag;  //ltag,rtag:0 point to child；1: point to prev or next node
}ThreadTreeNode;  //线索树结点

typedef ThreadTreeNode* ThreadTree; //Thread Tree
typedef BiTreeNode* BiTree; //Binary Tree

//Tree operations declarations
BiTree create_BiTree(void);
Status initialize_BiTree(BiTree *T);
Status destroy_BiTree(BiTree T);
Status inorder_Traversal(const BiTree T);
Status preorder_Traversal(const BiTree T);
Status postorder_Traversal(const BiTree T);
Status levelorder_Traversal(const BiTree T, BiTree queue[MAXSIZE]);//层序遍历 level-order
size_t get_BiTree_Depth(const BiTree T);

ThreadTree create_ThreadTree(void);//create thread bi-tree
Status initialize_ThreadTree(ThreadTree *T);
Status destroy_ThreadTree(ThreadTree T);
Status inorder_Threading(ThreadTree *Head, ThreadTree T);
Status inorder_Traversal_ThreadTree(const ThreadTree T);
//Status preorder_Traversal_ThreadTree(const ThreadTree T);
//Status postorder_Traversal_ThreadTree(const ThreadTree T);

#endif // TREE_H
