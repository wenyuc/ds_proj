/*
Red-Black Tree Unit Test
*/

#include "rbtree.h"

#include <stdio.h>
#include <stdlib.h>

/* ---------- 测试用例 1：基本插入 ---------- */
static void test_basic_insert(void) {
    printf("[1] 基本插入测试\n");
    RBTree *t = rb_create();
    int keys[] = {10, 20, 30, 15, 25, 5, 1, 40, 35, 50};
    int n = sizeof(keys) / sizeof(keys[0]);

    printf("    插入: ");
    for (int i = 0; i < n; i++) {
        printf("%d ", keys[i]);
        rb_insert(t, keys[i]);
    }
    printf("\n");

    printf("    树结构:\n");
    rb_print(t);

    printf("    中序遍历: ");
    rb_print_sorted(t);

    printf("    性质校验: %s\n\n", rb_validate(t) ? "通过" : "失败");

    rb_destroy(t);
}

/* ---------- 测试用例 2：查找 ---------- */
static void test_search(void) {
    printf("[2] 查找测试\n");
    RBTree *t = rb_create();
    int keys[] = {10, 20, 30, 15, 25, 5, 1, 40, 35, 50};
    int n = sizeof(keys) / sizeof(keys[0]);
    for (int i = 0; i < n; i++) rb_insert(t, keys[i]);

    int search_keys[] = {15, 99, 1, 50, 0};
    for (int i = 0; i < 5; i++) {
        RBNode *r = rb_search(t, search_keys[i]);
        if (r != t->nil)
            printf("    key=%d 找到, 颜色=%s\n", search_keys[i],
                   r->color == RED ? "红" : "黑");
        else
            printf("    key=%d 未找到\n", search_keys[i]);
    }
    printf("\n");

    rb_destroy(t);
}

/* ---------- 测试用例 3：删除 ---------- */
static void test_delete(void) {
    printf("[3] 删除测试\n");
    RBTree *t = rb_create();
    int keys[] = {10, 20, 30, 15, 25, 5, 1, 40, 35, 50};
    int n = sizeof(keys) / sizeof(keys[0]);
    for (int i = 0; i < n; i++) rb_insert(t, keys[i]);

    int del_keys[] = {30, 10, 50, 999, 1};
    for (int i = 0; i < 5; i++) {
        printf("    删除 %d ... ", del_keys[i]);
        rb_delete(t, del_keys[i]);
        CHECK(rb_validate(t),"性质校验");
    }
    printf("\n");

    printf("    删除后树结构:\n");
    rb_print(t);
    printf("    中序遍历: ");
    rb_print_sorted(t);
    printf("\n");

    rb_destroy(t);
}

/* ---------- 测试用例 4：随机压力测试 ---------- */
static void test_stress(void) {
    printf("[4] 随机压力测试 (10000 个节点)\n");
    RBTree *t = rb_create();
    srand(12345);
    const int N = 10000;
    int *arr = (int *)malloc(sizeof(int) * N);
    if (!arr) {
        fprintf(stderr, "malloc failed\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < N; i++) {
        arr[i] = rand() % 100000;
        rb_insert(t, arr[i]);
    }
    printf("    插入后校验: %s\n", rb_validate(t) ? "通过" : "失败");

    /* 删除一半 */
    for (int i = 0; i < N; i += 2)
        rb_delete(t, arr[i]);
    printf("    删除一半后校验: %s\n", rb_validate(t) ? "通过" : "失败");

    /* 校验剩余与删除状态 */
    int found = 0, leaked = 0;
    for (int i = 0; i < N; i++) {
        RBNode *r = rb_search(t, arr[i]);
        Bool exists = (r != t->nil);
        if (i % 2 == 0) {
            if (exists) leaked++;   /* 应已删除却仍存在 */
        } else {
            if (exists) found++;    /* 应仍存在 */
        }
    }
    printf("    剩余有效节点数: %d (期望 %d)\n", found, N / 2);
    printf("    已删除却仍存在的数: %d (期望 0)\n", leaked);
    printf("\n");

    free(arr);
    rb_destroy(t);
}

int main(void) {
    printf("=== 红黑树测试 ===\n\n");
    test_basic_insert();
    test_search();
    test_delete();
    test_stress();
    printf("=== 测试完成 ===\n");
    return 0;
}