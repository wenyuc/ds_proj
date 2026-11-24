/**
 * @file    btree.c
 * @brief   CLRS 风格 B树实现 (最小度数 t)
 *
 * 算法对应 CLRS:
 *   B-TREE-SEARCH         -> btree_search / node_find
 *   B-TREE-SPLIT-CHILD    -> bt_split_child
 *   B-TREE-INSERT-NONFULL -> bt_insert_nonfull
 *   B-TREE-INSERT         -> btree_insert
 *   B-TREE-DELETE         -> btree_delete 系列
 *     (含 DELETE-FROM-LEAF / INTERNAL / PREDECESSOR / SUCCESSOR
 *      / MERGE / BORROW-FROM-PREV / BORROW-FROM-NEXT)
 *
 * 与 CLRS 伪代码的唯一区别: 用池下标代替指针, 以便无动态内存。
 */

#include "btree.h"
#include <string.h>
#include <stdio.h>

/* ======================= 内部工具 ======================= */

static bnode_t *N(btree_t *t, int idx) { return &t->nodes[idx]; }

static int bt_alloc_node(btree_t *t)
{
    for (int i = 0; i < BTREE_MAX_NODES; i++) {
        if (!t->nodes[i].used) {
            bnode_t *x = &t->nodes[i];
            memset(x, 0, sizeof(*x));
            x->used = 1;
            x->leaf = 1;
            x->n    = 0;
            for (int j = 0; j < BTREE_MAX_CHILD; j++)
                x->child[j] = -1;
            t->free_cnt--;
            return i;
        }
    }
    return -1; /* 池满 */
}

static void bt_free_node(btree_t *t, int idx)
{
    if (idx < 0 || idx >= BTREE_MAX_NODES) return;
    t->nodes[idx].used = 0;
    t->free_cnt++;
}

/* 在节点 x 内找第一个 >= key 的下标 (CLRS 线性扫描) */
static int node_find(bnode_t *x, BTREE_KEY_T key)
{
    int i = 0;
    while (i < x->n && x->keys[i] < key) i++;
    return i;
}

/* ======================= 初始化 ======================= */

void btree_init(btree_t *t)
{
    memset(t, 0, sizeof(*t));
    t->root     = -1;
    t->free_cnt = BTREE_MAX_NODES;
}

/* ======================= 查找 ======================= */

int btree_search(btree_t *t, BTREE_KEY_T key)
{
    int cur = t->root;
    while (cur != -1) {
        bnode_t *x = N(t, cur);
        int i = node_find(x, key);
        if (i < x->n && x->keys[i] == key) return 1;
        if (x->leaf) return 0;
        cur = x->child[i];
    }
    return 0;
}

/* ======================= 分裂 ======================= */

/*
 * B-TREE-SPLIT-CHILD(x, i)
 * 前提: x 非满, y = x.child[i] 满 (n == 2t-1)
 * 结果: y 保留前 t-1 个 key, 新节点 z 拿后 t-1 个 key,
 *       中间 key y.keys[t-1] 上移到 x.keys[i],
 *       z 成为 x.child[i+1]。
 */
static int bt_split_child(btree_t *t, int x_idx, int i)
{
    bnode_t *x = N(t, x_idx);
    int y_idx  = x->child[i];
    bnode_t *y = N(t, y_idx);

    int z_idx = bt_alloc_node(t);
    if (z_idx < 0) return -1;
    bnode_t *z = N(t, z_idx);

    z->leaf = y->leaf;
    z->n    = BTREE_MIN_KEYS;            /* t-1 */

    /* z 拿 y 的后 t-1 个 key */
    for (int j = 0; j < BTREE_MIN_KEYS; j++)
        z->keys[j] = y->keys[j + BTREE_T];   /* y.keys[t .. 2t-2] */

    /* z 拿 y 的后 t 个孩子 */
    if (!y->leaf) {
        for (int j = 0; j < BTREE_T; j++)
            z->child[j] = y->child[j + BTREE_T];
    }

    y->n = BTREE_MIN_KEYS;               /* t-1 */

    /* x 中 key 和 child 后移一位, 插入中间 key 和 z */
    for (int j = x->n; j > i; j--)
        x->keys[j] = x->keys[j - 1];
    for (int j = x->n + 1; j > i; j--)
        x->child[j] = x->child[j - 1];

    x->keys[i]      = y->keys[BTREE_MIN_KEYS];   /* y.keys[t-1] 上移 */
    x->child[i + 1] = z_idx;
    x->n++;
    return 0;
}

/* ======================= 插入 ======================= */

/*
 * B-TREE-INSERT-NONFULL(x, k)
 * 前提: x 非满
 */
static int bt_insert_nonfull(btree_t *t, int x_idx, BTREE_KEY_T key)
{
    bnode_t *x = N(t, x_idx);
    int i = x->n - 1;

    if (x->leaf) {
        /* 叶: 直接有序插入, 重复忽略 */
        int pos = node_find(x, key);
        if (pos < x->n && x->keys[pos] == key) return BT_OK;
        for (; i >= pos; i--)
            x->keys[i + 1] = x->keys[i];
        x->keys[pos] = key;
        x->n++;
        return BT_OK;
    }

    /* 内部: 定位下降位置 */
    int pos = node_find(x, key);
    if (pos < x->n && x->keys[pos] == key) return BT_OK; /* 重复 */

    bnode_t *c = N(t, x->child[pos]);
    if (c->n == BTREE_MAX_KEYS) {
        /* 满则先分裂, 再重新定位 */
        if (bt_split_child(t, x_idx, pos) != 0) return BT_ENOMEM;
        if (key > x->keys[pos]) pos++;
    }
    return bt_insert_nonfull(t, x->child[pos], key);
}

int btree_insert(btree_t *t, BTREE_KEY_T key)
{
    /* 空树: 建根 */
    if (t->root == -1) {
        int r = bt_alloc_node(t);
        if (r < 0) return BT_ENOMEM;
        t->nodes[r].leaf    = 1;
        t->nodes[r].n       = 1;
        t->nodes[r].keys[0] = key;
        t->root = r;
        return BT_OK;
    }

    bnode_t *r = N(t, t->root);
    if (r->n == BTREE_MAX_KEYS) {
        /* 根满: 树高+1, 新根只含一个空孩子 */
        int s_idx = bt_alloc_node(t);
        if (s_idx < 0) return BT_ENOMEM;
        bnode_t *s = N(t, s_idx);
        s->leaf     = 0;
        s->n        = 0;
        s->child[0] = t->root;

        if (bt_split_child(t, s_idx, 0) != 0) {
            bt_free_node(t, s_idx);
            return BT_ENOMEM;
        }
        t->root = s_idx;
    }
    return bt_insert_nonfull(t, t->root, key);
}

/* ======================= 删除 ======================= */

/* MERGE(x, i): 合并 x.child[i] + x.keys[i] + x.child[i+1] 到 y, 删除 z */
static void bt_merge(btree_t *t, int x_idx, int i)
{
    bnode_t *x = N(t, x_idx);
    int y_idx = x->child[i];
    int z_idx = x->child[i + 1];
    bnode_t *y = N(t, y_idx);
    bnode_t *z = N(t, z_idx);

    /* y 接上 x.keys[i] */
    y->keys[y->n] = x->keys[i];
    y->n++;

    /* y 接上 z 的所有 key */
    for (int j = 0; j < z->n; j++)
        y->keys[y->n + j] = z->keys[j];

    /* y 接上 z 的所有孩子 */
    if (!y->leaf) {
        for (int j = 0; j <= z->n; j++)
            y->child[y->n + j] = z->child[j];
    }
    y->n += z->n;

    /* x 中移除 keys[i] 和 child[i+1] */
    for (int j = i; j < x->n - 1; j++)
        x->keys[j] = x->keys[j + 1];
    for (int j = i + 1; j < x->n; j++)
        x->child[j] = x->child[j + 1];
    x->n--;
    x->child[x->n + 1] = -1;

    bt_free_node(t, z_idx);
}

/* 从左兄弟借: x.child[i] 不足, 左兄弟够, 经 x.keys[i-1] 旋转 */
static void bt_borrow_from_prev(btree_t *t, int x_idx, int i)
{
    bnode_t *x = N(t, x_idx);
    bnode_t *c = N(t, x->child[i]);
    bnode_t *l = N(t, x->child[i - 1]);

    /* c 所有 key/child 右移一位 */
    for (int j = c->n; j > 0; j--)
        c->keys[j] = c->keys[j - 1];
    if (!c->leaf) {
        for (int j = c->n + 1; j > 0; j--)
            c->child[j] = c->child[j - 1];
        c->child[0] = l->child[l->n];
    }
    c->keys[0] = x->keys[i - 1];
    c->n++;

    x->keys[i - 1] = l->keys[l->n - 1];
    l->n--;
    if (!l->leaf) l->child[l->n + 1] = -1;
}

/* 从右兄弟借 */
static void bt_borrow_from_next(btree_t *t, int x_idx, int i)
{
    bnode_t *x = N(t, x_idx);
    bnode_t *c = N(t, x->child[i]);
    bnode_t *r = N(t, x->child[i + 1]);

    c->keys[c->n] = x->keys[i];
    if (!c->leaf)
        c->child[c->n + 1] = r->child[0];
    c->n++;

    x->keys[i] = r->keys[0];
    for (int j = 0; j < r->n - 1; j++)
        r->keys[j] = r->keys[j + 1];
    if (!r->leaf) {
        for (int j = 0; j < r->n; j++)
            r->child[j] = r->child[j + 1];
        r->child[r->n] = -1;
    }
    r->n--;
}

/*
 * 确保 x.child[i] 至少有 t 个 key, 供下降删除使用。
 * 对应 CLRS 的 "if c.n == t-1 then fill"。
 */
static void bt_fill(btree_t *t, int x_idx, int i)
{
    bnode_t *x = N(t, x_idx);
    if (i > 0 && N(t, x->child[i - 1])->n > BTREE_MIN_KEYS) {
        bt_borrow_from_prev(t, x_idx, i);
    } else if (i < x->n && N(t, x->child[i + 1])->n > BTREE_MIN_KEYS) {
        bt_borrow_from_next(t, x_idx, i);
    } else {
        if (i < x->n) bt_merge(t, x_idx, i);
        else          bt_merge(t, x_idx, i - 1);
    }
}

/* 子树最小 key(位于最左叶) */
static BTREE_KEY_T bt_min_key(btree_t *t, int x_idx)
{
    bnode_t *x = N(t, x_idx);
    while (!x->leaf) x = N(t, x->child[0]);
    return x->keys[0];
}

/* 子树最大 key(位于最右叶) */
static BTREE_KEY_T bt_max_key(btree_t *t, int x_idx)
{
    bnode_t *x = N(t, x_idx);
    while (!x->leaf) x = N(t, x->child[x->n]);
    return x->keys[x->n - 1];
}

/*
 * B-TREE-DELETE(x, k) 递归部分
 * 对应 CLRS 情况 1 / 2a / 2b / 2c / 3
 */
static void bt_delete(btree_t *t, int x_idx, BTREE_KEY_T key)
{
    bnode_t *x = N(t, x_idx);
    int i = node_find(x, key);

    if (i < x->n && x->keys[i] == key) {
        /* 情况 1: key 在 x 中 */
        if (x->leaf) {
            /* 1a: 叶, 直接删 */
            for (int j = i; j < x->n - 1; j++)
                x->keys[j] = x->keys[j + 1];
            x->n--;
        } else {
            int l_idx = x->child[i];
            int r_idx = x->child[i + 1];
            if (N(t, l_idx)->n >= BTREE_T) {
                /* 2a: 左孩子 >= t 个 key, 用前驱替换 */
                BTREE_KEY_T pred = bt_max_key(t, l_idx);
                x->keys[i] = pred;
                bt_delete(t, l_idx, pred);
            } else if (N(t, r_idx)->n >= BTREE_T) {
                /* 2b: 右孩子 >= t 个 key, 用后继替换 */
                BTREE_KEY_T succ = bt_min_key(t, r_idx);
                x->keys[i] = succ;
                bt_delete(t, r_idx, succ);
            } else {
                /* 2c: 左右都 t-1, 合并后再删 */
                bt_merge(t, x_idx, i);
                bt_delete(t, x->child[i], key);
            }
        }
        return;
    }

    /* 情况 3: key 不在 x */
    if (x->leaf) return;   /* 不存在 */

    /* 保证下降的孩子至少 t 个 key */
    int pos = i;
    if (N(t, x->child[pos])->n <= BTREE_MIN_KEYS) {
        bt_fill(t, x_idx, pos);
        /* 合并可能使 x.n 减少或结构变化, 重新定位 */
        if (x->n == 0) {
            /* 仅根会变空, 交给 btree_delete 处理; 此处继续到唯一孩子 */
            bt_delete(t, x->child[0], key);
            return;
        }
        pos = node_find(x, key);
        if (pos < x->n && x->keys[pos] == key)
            return; /* 不应发生(已在上面处理) */
    }
    bt_delete(t, x->child[pos], key);
}

int btree_delete(btree_t *t, BTREE_KEY_T key)
{
    if (t->root == -1) return 1;
    if (!btree_search(t, key)) return 1;

    bt_delete(t, t->root, key);

    /* 根变空: 降为唯一孩子, 或树空 */
    bnode_t *r = N(t, t->root);
    if (r->n == 0) {
        int old = t->root;
        if (r->leaf) {
            bt_free_node(t, old);
            t->root = -1;
        } else {
            t->root = r->child[0];
            bt_free_node(t, old);
        }
    }
    return BT_OK;
}

/* ======================= 遍历 ======================= */

static void bt_traverse(btree_t *t, int x_idx, void (*visit)(BTREE_KEY_T))
{
    bnode_t *x = N(t, x_idx);
    int i;
    for (i = 0; i < x->n; i++) {
        if (!x->leaf) bt_traverse(t, x->child[i], visit);
        visit(x->keys[i]);
    }
    if (!x->leaf) bt_traverse(t, x->child[i], visit);
}

void btree_traverse(btree_t *t, void (*visit)(BTREE_KEY_T))
{
    if (t->root == -1) return;
    bt_traverse(t, t->root, visit);
}

/* ======================= 校验 ======================= */

/*
 * 递归校验, 返回:
 *   0 合法
 *   1 非根 key 数越界
 *   2 根 key 数越界
 *   3 key 非升序
 *   4 叶节点却有孩子
 *   5 内部节点孩子指针为空 / 孩子数与 n 不匹配
 *   6/7 子树值域越界
 *   8 叶深度不一致
 * 通过 out_depth 返回该子树叶深度供上层比较。
 */
static int bt_check(btree_t *t, int x_idx, int is_root, int depth,
                    int *out_depth,
                    int has_lo, BTREE_KEY_T lo,
                    int has_hi, BTREE_KEY_T hi)
{
    bnode_t *x = N(t, x_idx);

    /* 1/2: key 数范围 */
    if (is_root) {
        if (x->n < 0 || x->n > BTREE_MAX_KEYS) return 2;
    } else {
        if (x->n < BTREE_MIN_KEYS || x->n > BTREE_MAX_KEYS) return 1;
    }

    /* 3: 升序 */
    for (int i = 1; i < x->n; i++)
        if (x->keys[i - 1] >= x->keys[i]) return 3;

    /* 6/7: 与父分界比较 */
    if (x->n > 0) {
        if (has_lo && x->keys[0] <= lo)             return 6;
        if (has_hi && x->keys[x->n - 1] >= hi)      return 7;
    }

    if (x->leaf) {
        /* 4: 叶不应有孩子 */
        for (int i = 0; i < BTREE_MAX_CHILD; i++)
            if (x->child[i] != -1) return 4;
        *out_depth = depth;
        return 0;
    }

    /* 5: 内部节点恰有 n+1 个孩子 */
    for (int i = 0; i <= x->n; i++)
        if (x->child[i] == -1) return 5;

    int first_depth = -1;
    for (int i = 0; i <= x->n; i++) {
        int d = -1;
        int has_lo2 = 1, has_hi2 = 1;
        BTREE_KEY_T lo2 = lo, hi2 = hi;
        if (i == 0)      has_lo2 = has_lo;
        else             { has_lo2 = 1; lo2 = x->keys[i - 1]; }
        if (i == x->n)   has_hi2 = has_hi;
        else             { has_hi2 = 1; hi2 = x->keys[i]; }

        int rc = bt_check(t, x->child[i], 0, depth + 1, &d,
                          has_lo2, lo2, has_hi2, hi2);
        if (rc) return rc;

        if (first_depth < 0) first_depth = d;
        else if (first_depth != d) return 8;  /* 叶深度不一致 */
    }
    *out_depth = first_depth;
    return 0;
}

int btree_validate(btree_t *t)
{
    if (t->root == -1) return 0;
    int d = 0;
    return bt_check(t, t->root, 1, 0, &d, 0, 0, 0, 0);
}

/* ======================= 调试 ======================= */

static void print_key(BTREE_KEY_T k) { printf("%d ", k); }

void btree_print(btree_t *t)
{
    printf("B-Tree keys: ");
    btree_traverse(t, print_key);
    printf("\n");
}
