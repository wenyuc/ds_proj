/*
Unit Test Mem Pool
*/

#include "ds_type.h"
#include "mem_pool.h"
#include <stdlib.h>
#include <string.h>

/* ---- 测试用配置 ---- */
#define BLOCK_SIZE  32
#define BLOCK_CNT   16
#define BUF_SIZE    (BLOCK_SIZE * BLOCK_CNT)

static mem_pool_t g_pool;
static uint8_t    g_buffer[BUF_SIZE + 16];  /* 多留点，测试对齐 */

static void test_basic(void)
{
    int status = mem_pool_init(&g_pool, g_buffer, sizeof(g_buffer), BLOCK_SIZE);
    CHECK(status == 0, "initialize pool");
    if (status != 0) return;

    void *ptrs[BLOCK_CNT];
    for (int i = 0; i < BLOCK_CNT; i++) {
        ptrs[i] = mem_pool_alloc(&g_pool);
        CHECK(ptrs[i] != NULL, "ptrs[i] != NULL");
        if (ptrs[i] == NULL) return;
        memset(ptrs[i], 0xA5, BLOCK_SIZE);
    }

    /* 池应当已满 */
    CHECK(mem_pool_alloc(&g_pool) == NULL, "mem_pool_alloc(&g_pool) == NULL");

    mem_pool_stats_t s;
    mem_pool_get_stats(&g_pool, &s);
    CHECK(s.used_blocks == s.total_blocks, "s.used_blocks == s.total_blocks");
    CHECK(s.fail_count == 1, "s.fail_count == 1");

    /* 释放一半，再分配一半 */
    for (int i = 0; i < BLOCK_CNT / 2; i++) {
        mem_pool_free(&g_pool, ptrs[i]);
    }
    for (int i = 0; i < BLOCK_CNT / 2; i++) {
        void *p = mem_pool_alloc(&g_pool);
        CHECK(p != NULL, "p != NULL");
        if (p == NULL) return;
        mem_pool_free(&g_pool, p);
    }

    /* 释放全部 */
    for (int i = BLOCK_CNT / 2; i < BLOCK_CNT; i++)
        mem_pool_free(&g_pool, ptrs[i]);

    mem_pool_get_stats(&g_pool, &s);
    CHECK(s.used_blocks == 0, "s.used_blocks == 0");
    /* reset 后再用 */
    mem_pool_reset(&g_pool);
    void *p = mem_pool_alloc(&g_pool);
    CHECK(p != NULL, "pool allocates after reset");
}

static void test_alignment(void)
{
    /* 故意给一个不对齐的缓冲区起点 */
    uint8_t *raw = g_buffer;
    for (size_t off = 1; off < 8; off++) {
        mem_pool_t p;
        int rc = mem_pool_init(&p, raw + off, BUF_SIZE, BLOCK_SIZE);
        CHECK(rc == 0, "initialize misaligned buffer");
        if (rc != 0) continue;
        void *a = mem_pool_alloc(&p);
        CHECK(a != NULL, "a != NULL");
        if (a == NULL) continue;
        CHECK(((uintptr_t)a % MEM_POOL_ALIGN) == 0, "((uintptr_t)a % MEM_POOL_ALIGN) == 0");
        mem_pool_free(&p, a);
    }
}

static void test_illegal_free(void)
{
    int status = mem_pool_init(&g_pool, g_buffer, sizeof(g_buffer), BLOCK_SIZE);
    CHECK(status == 0, "initialize pool for invalid frees");
    if (status != 0) return;

    int dummy;
    /* 非法地址不应崩溃 */
    mem_pool_free(&g_pool, &dummy);
    mem_pool_free(&g_pool, NULL);

    mem_pool_stats_t s;
    mem_pool_get_stats(&g_pool, &s);
    CHECK(s.used_blocks == 0, "s.used_blocks == 0");
}

/* 压力/随机测试 */
static void test_stress(void)
{
    int status = mem_pool_init(&g_pool, g_buffer, sizeof(g_buffer), BLOCK_SIZE);
    CHECK(status == 0, "initialize pool for stress test");
    if (status != 0) return;

    enum { N = 64 };
    void *ptrs[N] = {0};
    srand(12345);

    for (int iter = 0; iter < 100000; iter++) {
        int idx = rand() % N;
        if (ptrs[idx]) {
            mem_pool_free(&g_pool, ptrs[idx]);
            ptrs[idx] = NULL;
        } else {
            ptrs[idx] = mem_pool_alloc(&g_pool);
        }
    }
    /* 清理 */
    for (int i = 0; i < N; i++)
        if (ptrs[i]) mem_pool_free(&g_pool, ptrs[i]);

    mem_pool_stats_t s;
    mem_pool_get_stats(&g_pool, &s);
    CHECK(s.used_blocks == 0, "s.used_blocks == 0");
}

int main(void)
{
    test_basic();
    test_alignment();
    test_illegal_free();
    test_stress();
    return g_fail == 0 ? 0 : 1;
}
