/*
Red-Black Tree Header
*/

#ifndef RBTREE_H
#define RBTREE_H

#include "ds_type.h"

/* ==================== 数据结构定义 ==================== */

typedef struct rbnode {
    int key;
    Color color;
    struct rbnode *left;
    struct rbnode *right;
    struct rbnode *parent;
} RBNode;

typedef struct {
    RBNode *root;
    RBNode *nil;   /* 哨兵节点，代表所有叶子节点和根的父亲 */
} RBTree;

/* ==================== 生命周期 ==================== */

/* 创建一棵空红黑树（内部会分配哨兵节点） */
RBTree *rb_create(void);

/* 销毁整棵树，释放所有节点内存 */
void rb_destroy(RBTree *t);

/* ==================== 基本操作 ==================== */

/* 插入 key；若 key 已存在则不重复插入 */
void rb_insert(RBTree *t, int key);

/* 删除 key；若不存在则无操作 */
void rb_delete(RBTree *t, int key);

/* 查找 key，返回节点指针；未找到返回 t->nil */
RBNode *rb_search(RBTree *t, int key);

/* ==================== 校验与打印 ==================== */

/* 检查是否满足红黑树五条性质，返回 true 表示合法 */
Bool rb_validate(RBTree *t);

/* 以树形打印（逆时针旋转 90 度视角，右子树在上） */
void rb_print(RBTree *t);

/* 中序遍历打印，应输出升序结果 */
void rb_print_sorted(RBTree *t);

#endif /* RBTREE_H */