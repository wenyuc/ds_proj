/*
Mem Pool Implementation
*/

#include "mem_pool.h"
#include <string.h>

/* 向上对齐 
把 v 向上取整到 a 的整数倍（a 必须是 2 的幂）。
功能等价于 ((v + a - 1) / a) * a，但用位运算实现，无除法、无分支，速度更快。
*/
static inline size_t align_up(size_t v, size_t a) {
    return (v + a - 1) & ~(a - 1);
}

#ifdef MEM_POOL_THREAD_SAFE
static inline void mp_lock(mem_pool_t *p) {
    if (p->lock) p->lock(p->lock_ctx);
}
static inline void mp_unlock(mem_pool_t *p) {
    if (p->unlock) p->unlock(p->lock_ctx);
}
#else
static inline void mp_lock(mem_pool_t *p)   { (void)p; } //无锁版本是空操作
static inline void mp_unlock(mem_pool_t *p) { (void)p; }
#endif

int mem_pool_init(mem_pool_t *pool,
                  void *buffer,
                  size_t buf_size,
                  size_t block_size)
{
    if (!pool || !buffer || block_size < sizeof(free_node_t))
        return -1;

    /* 对块大小向上对齐 */
    block_size = align_up(block_size, MEM_POOL_ALIGN);

    /* 对缓冲区起始地址对齐 */
    uintptr_t addr = (uintptr_t)buffer;
    uintptr_t aligned = (addr + (MEM_POOL_ALIGN - 1)) & ~(uintptr_t)(MEM_POOL_ALIGN - 1);
    size_t    skip = aligned - addr;

    if (buf_size <= skip) return -2;
    size_t usable = buf_size - skip;

    size_t n = usable / block_size;
    if (n == 0) return -3;

    memset(pool, 0, sizeof(*pool));
    pool->base         = (uint8_t *)aligned;
    pool->block_size   = block_size;
    pool->total_blocks = n;
    pool->used_blocks  = 0;
    pool->peak_used    = 0;
    pool->fail_count   = 0;

    /* 构造空闲链表 */
    pool->free_list = NULL;
    for (size_t i = 0; i < n; i++) {
        free_node_t *node = (free_node_t *)(pool->base + i * block_size);
        node->next = pool->free_list;
        pool->free_list = node;
    }
    return 0;
}

void *mem_pool_alloc(mem_pool_t *pool)
{
    if (!pool) return NULL;

    mp_lock(pool);
    free_node_t *node = pool->free_list;
    if (!node) {
        pool->fail_count++;
        mp_unlock(pool);
        return NULL;
    }
    pool->free_list = node->next;

    pool->used_blocks++;
    if (pool->used_blocks > pool->peak_used)
        pool->peak_used = pool->used_blocks;
    mp_unlock(pool);

    return (void *)node;
}

void mem_pool_free(mem_pool_t *pool, void *ptr)
{
    if (!pool || !ptr) return;

    /* 简单边界检查（可选，调试用） */
    uintptr_t p = (uintptr_t)ptr;
    uintptr_t base = (uintptr_t)pool->base;
    if (p < base ||
        p >= base + pool->total_blocks * pool->block_size ||
        ((p - base) % pool->block_size) != 0)
    {
        return; /* 非法指针，忽略 */
    }

    mp_lock(pool);
    free_node_t *node = (free_node_t *)ptr;
    node->next = pool->free_list;
    pool->free_list = node;
    if (pool->used_blocks > 0) pool->used_blocks--;
    mp_unlock(pool);
}

void mem_pool_get_stats(mem_pool_t *pool, mem_pool_stats_t *out)
{
    if (!pool || !out) return;
    mp_lock(pool);
    out->total_blocks = pool->total_blocks;
    out->used_blocks  = pool->used_blocks;
    out->peak_used    = pool->peak_used;
    out->fail_count   = pool->fail_count;
    mp_unlock(pool);
}

void mem_pool_reset(mem_pool_t *pool)
{
    if (!pool) return;
    mp_lock(pool);
    pool->free_list = NULL;
    for (size_t i = 0; i < pool->total_blocks; i++) {
        free_node_t *node = (free_node_t *)(pool->base + i * pool->block_size);
        node->next = pool->free_list;
        pool->free_list = node;
    }
    pool->used_blocks = 0;
    /* peak_used/fail_count 不清零，保留历史 */
    mp_unlock(pool);
}

#ifdef MEM_POOL_THREAD_SAFE
void mem_pool_set_lock(mem_pool_t *pool,
                       mp_lock_fn lock, mp_unlock_fn unlock, void *ctx)
{
    if (!pool) return;
    pool->lock = lock;
    pool->unlock = unlock;
    pool->lock_ctx = ctx;
}
#endif