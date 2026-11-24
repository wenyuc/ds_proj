/**
 * @file    btree_test.c
 * @brief   B树测试: 功能 + 结构校验 + 压力
 */
#include "ds_type.h"
#include "btree.h"
#include <stdio.h>

#define TEST_KEY_MAX 256

static int g_fail = 0; //fail testcase count

//helper function in unit test
#define CHECK(cond, msg) do { \
    if (!(cond)) { printf("FAIL: %s\n", msg); g_fail++; } \
    else { printf("PASS: %s\n", msg); } \
} while (0)

int main(void)
{
    static btree_t tree;

    printf("=== CLRS B-Tree Test (t=%d, key:[%d,%d], child:[%d,%d]) ===\n",
           BTREE_T, BTREE_MIN_KEYS, BTREE_MAX_KEYS,
           BTREE_MIN_CHILD, BTREE_MAX_CHILD);

    /* 1. 初始化 */
    btree_init(&tree);
    CHECK(btree_validate(&tree) == 0, "init validate");
    printf("[ OK ] init\n");

    /* 2. 插入 0..99 */
    for (int i = 0; i < 100; i++)
        CHECK(btree_insert(&tree, i) == BT_OK, "insert");
    CHECK(btree_validate(&tree) == 0, "validate after insert");
    printf("[ OK ] insert 0..99, validate\n");

    /* 3. 查找存在 */
    for (int i = 0; i < 100; i++)
        CHECK(btree_search(&tree, i) == 1, "search present");
    printf("[ OK ] search 0..99 all found\n");

    /* 4. 查找不存在 */
    for (int i = 100; i < 200; i++)
        CHECK(btree_search(&tree, i) == 0, "search absent");
    printf("[ OK ] search absent rejected\n");

    /* 5. 重复插入忽略 */
    CHECK(btree_insert(&tree, 50) == BT_OK, "dup insert");
    CHECK(btree_validate(&tree) == 0, "validate after dup");
    printf("[ OK ] duplicate insert ignored\n");

    /* 6. 删除奇数 */
    for (int i = 1; i < 100; i += 2)
        CHECK(btree_delete(&tree, i) == BT_OK, "delete odd");
    CHECK(btree_validate(&tree) == 0, "validate after delete odd");
    printf("[ OK ] delete odd, validate\n");

    /* 7. 奇偶一致性 */
    for (int i = 0; i < 100; i++)
        CHECK(btree_search(&tree, i) == (i % 2 == 0), "post-delete search");
    printf("[ OK ] post-delete search consistent\n");

    /* 8. 删光偶数 */
    for (int i = 0; i < 100; i += 2)
        CHECK(btree_delete(&tree, i) == BT_OK, "delete even");
    CHECK(btree_validate(&tree) == 0, "validate empty");
    CHECK(tree.root == -1, "root == -1 after empty");
    printf("[ OK ] tree emptied\n");

    /* 9. 压力: 随机插删, 每步校验 */
    printf("--- stress ---\n");
    btree_init(&tree);
    unsigned seed = 12345;
    for (int round = 0; round < 500; round++) {
        seed = seed * 1103515245u + 12345u;
        int k = (int)((seed >> 16) % TEST_KEY_MAX);
        if (btree_search(&tree, k)) btree_delete(&tree, k);
        else                        btree_insert(&tree, k);
        int rc = btree_validate(&tree);
        if (rc) {
            printf("[FAIL] stress round %d rc=%d key=%d\n", round, rc, k);
            g_fail++;
            break;
        }
    }
    if (!g_fail) printf("[ OK ] stress 500 ops\n");

    btree_print(&tree);
    printf("\n=== %s ===\n", g_fail ? "FAILED" : "ALL TESTS PASSED");
    return g_fail? 1 : 0;
}