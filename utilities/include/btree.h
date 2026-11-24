/**
 * @file    btree.h
 * @brief   CLRS 风格 B树（最小度数 t），嵌入式静态内存池实现
 *
 * 遵循《算法导论(CLRS)》第18章定义:
 *   - 最小度数 t = BTREE_T
 *   - 每节点 key 数: 非根 [t-1, 2t-1], 根 [0, 2t-1]
 *   - 每节点子节点数: 非根 [t, 2t], 根 [0, 2t]
 *   - 所有叶子同深度, key 升序, 子树按 key 分界
 *
 * 特性:
 *   - 无 malloc/free, 节点来自静态池
 *   - "指针"用池数组下标表示, -1 表示空
 *   - 可重入: 状态全在 btree_t 实例中, 无全局变量
 *   - 支持 insert / search / delete / traverse / validate
 */
#ifndef BTREE_H
#define BTREE_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ======================= 配置 ======================= */
#ifndef BTREE_T
#define BTREE_T            2          /* 最小度数 t (CLRS), 必须 >= 2 */
#endif

#define BTREE_MAX_KEYS     (2 * BTREE_T - 1)   /* 2t-1: key 上限 */
#define BTREE_MIN_KEYS     (BTREE_T - 1)       /* t-1 : key 下限(非根) */
#define BTREE_MAX_CHILD    (2 * BTREE_T)       /* 2t  : 子节点上限 */
#define BTREE_MIN_CHILD    (BTREE_T)           /* t   : 子节点下限(非根) */

#ifndef BTREE_MAX_NODES
#define BTREE_MAX_NODES    128        /* 静态节点池容量 */
#endif

#ifndef BTREE_KEY_T
#define BTREE_KEY_T        int        /* key 类型, 需支持 < > == */
#endif

/* 返回码 */
#define BT_OK        0
#define BT_ENOMEM   -1   /* 节点池耗尽 */
#define BT_EINVAL   -2   /* 参数非法 */

/* ======================= 类型 ======================= */
typedef struct {
    int        used;                        /* 池占用标志 */
    int        n;                           /* 当前 key 数 x.n */
    int        leaf;                        /* 1=叶, 0=内部 */
    BTREE_KEY_T keys[BTREE_MAX_KEYS];       /* 升序 key */
    int        child[BTREE_MAX_CHILD];      /* 子节点池下标, -1 空 */
} bnode_t;

typedef struct {
    bnode_t nodes[BTREE_MAX_NODES];         /* 节点池 */
    int     root;                           /* 根下标, -1 空树 */
    int     free_cnt;                       /* 空闲节点数 */
} btree_t;

/* ======================= API ======================= */

/** 初始化空树 */
void btree_init(btree_t *t);

/** 查找: 找到返回 1, 否则 0 */
int  btree_search(btree_t *t, BTREE_KEY_T key);

/** 插入: BT_OK 成功, 重复 key 也返回 BT_OK(忽略), BT_ENOMEM 池满 */
int  btree_insert(btree_t *t, BTREE_KEY_T key);

/** 删除: BT_OK 成功, 1 表示 key 不存在 */
int  btree_delete(btree_t *t, BTREE_KEY_T key);

/** 中序遍历(升序), 对每个 key 调用 visit */
void btree_traverse(btree_t *t, void (*visit)(BTREE_KEY_T key));

/** 结构校验: BT_OK 合法, 否则返回非0 错误码(见 btree.c 注释) */
int  btree_validate(btree_t *t);

/** 调试: 打印所有 key(升序) */
void btree_print(btree_t *t);

#ifdef __cplusplus
}
#endif

#endif /* BTREE_H */