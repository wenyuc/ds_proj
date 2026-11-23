/*
固定块内存池（Fixed-Size Memory Pool）实现。
它避免了 malloc/free 的碎片化问题，分配/释放是 O(1)，
且不需要动态内存，全部静态分配，适合 RTOS 和裸机环境。

设计思路：
内存池 = 预分配的连续大缓冲区 + 空闲块链表

[ Block0 | Block1 | Block2 | ... | BlockN ]
   ↓
空闲时：每个块的前几个字节存放下一个空闲块的指针（复用块本身）
已分配：整块交给用户

*/

#ifndef MEM_POOL_H
#define MEM_POOL_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ---------- 配置 ---------- */
#ifndef MEM_POOL_ALIGN
#define MEM_POOL_ALIGN  (sizeof(void *))   /* 对齐粒度，可改为 8/16 */
#endif

/* 线程安全钩子：如需在 RTOS 中使用，定义它们 */
#ifdef MEM_POOL_THREAD_SAFE
typedef void (*mp_lock_fn)(void *ctx);
typedef void (*mp_unlock_fn)(void *ctx);
#endif

/* 空闲块节点，复用在每个块的头部 */
typedef struct free_node {
    struct free_node *next;
} free_node_t;

typedef struct {
    uint8_t   *base;         /* 缓冲区起点（对齐后） */
    size_t     block_size;   /* 每块实际大小（已对齐） */
    size_t     total_blocks;
    size_t     used_blocks;
    size_t     peak_used;
    size_t     fail_count;
    free_node_t *free_list;

#ifdef MEM_POOL_THREAD_SAFE
    mp_lock_fn   lock;
    mp_unlock_fn unlock;
    void        *lock_ctx;
#endif
}mem_pool_t;

/* 统计信息 */
typedef struct {
    size_t total_blocks;
    size_t used_blocks;
    size_t peak_used;
    size_t fail_count;
} mem_pool_stats_t;

/* ---------- API ---------- */

/**
 * @brief 初始化内存池
 * @param pool      内存池对象指针（用户提供，静态分配）
 * @param buffer    底层缓冲区（必须对齐）
 * @param buf_size  缓冲区字节数
 * @param block_size 每块字节数（会自动按 MEM_POOL_ALIGN 向上对齐）
 * @return 0 成功，<0 失败
 */
int mem_pool_init(mem_pool_t *pool,
                  void *buffer,
                  size_t buf_size,
                  size_t block_size);

/* 分配一块内存；返回 NULL 表示池已空 */
void *mem_pool_alloc(mem_pool_t *pool);

/* 释放一块内存 */
void mem_pool_free(mem_pool_t *pool, void *ptr);

/* 获取统计信息 */
void mem_pool_get_stats(mem_pool_t *pool, mem_pool_stats_t *out);

/* 清空整个池（危险：所有已分配块都会被回收） */
void mem_pool_reset(mem_pool_t *pool);

#ifdef MEM_POOL_THREAD_SAFE
void mem_pool_set_lock(mem_pool_t *pool,
                       mp_lock_fn lock, mp_unlock_fn unlock, void *ctx);
#endif

#ifdef __cplusplus
}
#endif

#endif /* MEM_POOL_H */