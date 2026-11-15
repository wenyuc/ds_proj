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
}TreeNode;  //二叉树结点

typedef struct indexnode
{
    ElemType data;
    struct indexnode *lchild;
    struct indexnode *rchild;
    int ltag,rtag;  //ltag,rtag:0 point to child；1: point to prev or next node
}IndexNode;  //线索树结点

typedef IndexNode* InTree; //Index Tree
typedef TreeNode* BiTree; //Binary Tree

//Tree operations declarations
Status create_BiTree(BiTree *T);
Status destroy_BiTree(BiTree T);
Status create_InTree(InTree *T);//create index bi-tree
Status destroy_InTree(InTree T);
Status display_BiTree(BiTree T);
Status display_InTree(InTree T);
Status inorder_Traversal(const BiTree T);
Status preorder_Traversal(const BiTree T);
Status postorder_Traversal(const BiTree T);
Status inorder_Traversal_InTree(const InTree T);
Status preorder_Traversal_InTree(const InTree T);
Status postorder_Traversal_InTree(const InTree T);

#endif // TREE_H