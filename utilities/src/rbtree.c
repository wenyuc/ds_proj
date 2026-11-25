/*
Red-Black Tree Implementation
*/
#include "rbtree.h"

#include <stdio.h>
#include <stdlib.h>

/* ==================== 辅助函数 ==================== */

static RBNode *rb_new_node(RBTree *t, int key) {
    RBNode *n = (RBNode *)malloc(sizeof(RBNode));
    if (!n) {
        fprintf(stderr, "malloc failed\n");
        exit(EXIT_FAILURE);
    }
    n->key = key;
    n->color = RED;      /* 新节点默认红色 */
    n->left = t->nil;
    n->right = t->nil;
    n->parent = t->nil;
    return n;
}

RBTree *rb_create(void) {
    RBTree *t = (RBTree *)malloc(sizeof(RBTree));
    if (!t) {
        fprintf(stderr, "malloc failed\n");
        exit(EXIT_FAILURE);
    }
    t->nil = (RBNode *)malloc(sizeof(RBNode));
    if (!t->nil) {
        fprintf(stderr, "malloc failed\n");
        exit(EXIT_FAILURE);
    }
    t->nil->color = BLACK;
    t->nil->left = t->nil->right = t->nil->parent = NULL;
    t->nil->key = 0;
    t->root = t->nil;
    return t;
}

static void rb_destroy_node(RBTree *t, RBNode *n) {
    if (n != t->nil) {
        rb_destroy_node(t, n->left);
        rb_destroy_node(t, n->right);
        free(n);
    }
}

void rb_destroy(RBTree *t) {
    if (!t) return;
    rb_destroy_node(t, t->root);
    free(t->nil);
    free(t);
}

/* ==================== 旋转操作 ==================== */

static void rb_left_rotate(RBTree *t, RBNode *x) {
    RBNode *y = x->right;
    x->right = y->left;
    if (y->left != t->nil)
        y->left->parent = x;

    y->parent = x->parent;
    if (x->parent == t->nil)
        t->root = y;
    else if (x == x->parent->left)
        x->parent->left = y;
    else
        x->parent->right = y;

    y->left = x;
    x->parent = y;
}

static void rb_right_rotate(RBTree *t, RBNode *x) {
    RBNode *y = x->left;
    x->left = y->right;
    if (y->right != t->nil)
        y->right->parent = x;

    y->parent = x->parent;
    if (x->parent == t->nil)
        t->root = y;
    else if (x == x->parent->right)
        x->parent->right = y;
    else
        x->parent->left = y;

    y->right = x;
    x->parent = y;
}

/* ==================== 插入修复 ==================== */

static void rb_insert_fixup(RBTree *t, RBNode *z) {
    while (z->parent->color == RED) {
        if (z->parent == z->parent->parent->left) {
            RBNode *y = z->parent->parent->right;   /* 叔叔节点 */
            if (y->color == RED) {
                /* Case 1: 叔叔为红 */
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            } else {
                if (z == z->parent->right) {
                    /* Case 2: 叔叔为黑，z 是右孩子 */
                    z = z->parent;
                    rb_left_rotate(t, z);
                }
                /* Case 3: 叔叔为黑，z 是左孩子 */
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                rb_right_rotate(t, z->parent->parent);
            }
        } else {
            /* 镜像情况 */
            RBNode *y = z->parent->parent->left;
            if (y->color == RED) {
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            } else {
                if (z == z->parent->left) {
                    z = z->parent;
                    rb_right_rotate(t, z);
                }
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                rb_left_rotate(t, z->parent->parent);
            }
        }
    }
    t->root->color = BLACK;
}

/* ==================== 插入 ==================== */

void rb_insert(RBTree *t, int key) {
    RBNode *z = rb_new_node(t, key);
    RBNode *y = t->nil;
    RBNode *x = t->root;

    while (x != t->nil) {
        y = x;
        if (z->key < x->key)
            x = x->left;
        else if (z->key > x->key)
            x = x->right;
        else {
            /* 已存在相同 key，简单丢弃（也可改为允许重复） */
            free(z);
            return;
        }
    }

    z->parent = y;
    if (y == t->nil)
        t->root = z;
    else if (z->key < y->key)
        y->left = z;
    else
        y->right = z;

    z->left = t->nil;
    z->right = t->nil;
    z->color = RED;
    rb_insert_fixup(t, z);
}

/* ==================== 查找 ==================== */

RBNode *rb_search(RBTree *t, int key) {
    RBNode *x = t->root;
    while (x != t->nil && x->key != key) {
        if (key < x->key)
            x = x->left;
        else
            x = x->right;
    }
    return x;
}

static RBNode *rb_minimum(RBTree *t, RBNode *x) {
    while (x->left != t->nil)
        x = x->left;
    return x;
}

/* 用 v 替换 u（仅调整父子指针） */
static void rb_transplant(RBTree *t, RBNode *u, RBNode *v) {
    if (u->parent == t->nil)
        t->root = v;
    else if (u == u->parent->left)
        u->parent->left = v;
    else
        u->parent->right = v;
    v->parent = u->parent;
}

/* ==================== 删除修复 ==================== */

static void rb_delete_fixup(RBTree *t, RBNode *x) {
    while (x != t->root && x->color == BLACK) {
        if (x == x->parent->left) {
            RBNode *w = x->parent->right;   /* 兄弟 */
            if (w->color == RED) {
                /* Case 1 */
                w->color = BLACK;
                x->parent->color = RED;
                rb_left_rotate(t, x->parent);
                w = x->parent->right;
            }
            if (w->left->color == BLACK && w->right->color == BLACK) {
                /* Case 2 */
                w->color = RED;
                x = x->parent;
            } else {
                if (w->right->color == BLACK) {
                    /* Case 3 */
                    w->left->color = BLACK;
                    w->color = RED;
                    rb_right_rotate(t, w);
                    w = x->parent->right;
                }
                /* Case 4 */
                w->color = x->parent->color;
                x->parent->color = BLACK;
                w->right->color = BLACK;
                rb_left_rotate(t, x->parent);
                x = t->root;
            }
        } else {
            /* 镜像情况 */
            RBNode *w = x->parent->left;
            if (w->color == RED) {
                w->color = BLACK;
                x->parent->color = RED;
                rb_right_rotate(t, x->parent);
                w = x->parent->left;
            }
            if (w->right->color == BLACK && w->left->color == BLACK) {
                w->color = RED;
                x = x->parent;
            } else {
                if (w->left->color == BLACK) {
                    w->right->color = BLACK;
                    w->color = RED;
                    rb_left_rotate(t, w);
                    w = x->parent->left;
                }
                w->color = x->parent->color;
                x->parent->color = BLACK;
                w->left->color = BLACK;
                rb_right_rotate(t, x->parent);
                x = t->root;
            }
        }
    }
    x->color = BLACK;
}

void rb_delete(RBTree *t, int key) {
    RBNode *z = rb_search(t, key);
    if (z == t->nil) return;   /* 未找到 */

    RBNode *y = z;
    RBNode *x;
    Color y_original_color = y->color;

    if (z->left == t->nil) {
        x = z->right;
        rb_transplant(t, z, z->right);
    } else if (z->right == t->nil) {
        x = z->left;
        rb_transplant(t, z, z->left);
    } else {
        y = rb_minimum(t, z->right);
        y_original_color = y->color;
        x = y->right;
        if (y->parent == z) {
            x->parent = y;
        } else {
            rb_transplant(t, y, y->right);
            y->right = z->right;
            y->right->parent = y;
        }
        rb_transplant(t, z, y);
        y->left = z->left;
        y->left->parent = y;
        y->color = z->color;
    }

    free(z);
    if (y_original_color == BLACK)
        rb_delete_fixup(t, x);
}

/* ==================== 校验与打印 ==================== */

/* 返回黑高，若违反性质返回 -1 */
static int rb_check(RBTree *t, RBNode *n) {
    if (n == t->nil) return 1;

    if (n->color == RED) {
        if (n->left->color == RED || n->right->color == RED)
            return -1;
    }

    int lh = rb_check(t, n->left);
    int rh = rb_check(t, n->right);
    if (lh < 0 || rh < 0 || lh != rh) return -1;

    return lh + (n->color == BLACK ? 1 : 0);
}

Bool rb_validate(RBTree *t) {
    if (t->root->color != BLACK) return FALSE;
    return rb_check(t, t->root) > 0;
}

static void rb_print_helper(RBTree *t, RBNode *n, int depth) {
    if (n == t->nil) return;
    rb_print_helper(t, n->right, depth + 1);
    for (int i = 0; i < depth; i++) printf("    ");
    printf("%d(%s)\n", n->key, n->color == RED ? "R" : "B");
    rb_print_helper(t, n->left, depth + 1);
}

void rb_print(RBTree *t) {
    printf("---- RBTree (逆时针旋转90度看) ----\n");
    rb_print_helper(t, t->root, 0);
    printf("------------------------------------\n");
}

static void rb_inorder(RBTree *t, RBNode *n) {
    if (n == t->nil) return;
    rb_inorder(t, n->left);
    printf("%d ", n->key);
    rb_inorder(t, n->right);
}

void rb_print_sorted(RBTree *t) {
    rb_inorder(t, t->root);
    printf("\n");
}