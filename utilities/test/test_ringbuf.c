/*
RingBuf Unit Test 
*/

#include "ringbuf.h"

void rb_test_cases(void) {
    /* 静态分配：16 字节，有效容量 15 */
    uint8_t storage[16];
    RingBuf rb;
    rb_init(&rb, storage, sizeof(storage));

    printf("=== 基本读写 ===\n");
    CHECK(rb_put(&rb, 0x11) == SUCCESS, "put 0x11");
    CHECK(rb_put(&rb, 0x22) == SUCCESS, "put 0x22");
    CHECK(rb_put(&rb, 0x33) == SUCCESS, "put 0x33");

    uint8_t b;
    CHECK(rb_get(&rb, &b) == SUCCESS && b == 0x11, "get 0x11");
    CHECK(rb_get(&rb, &b) == SUCCESS && b == 0x22, "get 0x22");
    CHECK(rb_get(&rb, &b) == SUCCESS && b == 0x33, "get 0x33");
    CHECK(rb_get(&rb, &b) == ERROR, "get on empty returns -1");

    printf("\n=== 判满（有效容量 15）===\n");
    rb_reset(&rb);
    for (int i = 0; i < 15; i++) {
        CHECK(rb_put(&rb, (uint8_t)i) == SUCCESS, "put until full");
    }
    CHECK(rb_put(&rb, 0xFF) == ERROR, "put on full returns -1");
    CHECK(rb_available(&rb) == 15, "available == 15 when full");
    CHECK(rb_free(&rb) == SUCCESS, "free == 0 when full");

    printf("\n=== 回绕（读写超过 size）===\n");
    rb_reset(&rb);
    /* 先写满 15，再读 10，再写 10，验证回绕 */
    for (int i = 0; i < 15; i++) rb_put(&rb, (uint8_t)i);
    for (int i = 0; i < 10; i++) {
        CHECK(rb_get(&rb, &b) == SUCCESS && b == (uint8_t)i, "get after wrap");
    }
    for (int i = 0; i < 10; i++) {
        CHECK(rb_put(&rb, (uint8_t)(0x80 + i)) == SUCCESS, "put after wrap");
    }
    /* 现在应剩 5（旧 5..14）+ 10（0x80..0x89）= 15，满 */
    CHECK(rb_available(&rb) == 15, "available == 15 after wrap");
    for (int i = 10; i < 15; i++) {  /* 旧数据10..14,5个*/
        CHECK(rb_get(&rb, &b) == SUCCESS && b == (uint8_t)i, "get old data after wrap");
    }
    for (int i = 0; i < 10; i++) {//新数据0x80..0x89 10个
        CHECK(rb_get(&rb, &b) == SUCCESS && b == (uint8_t)(0x80 + i), "get new data after wrap");
    }
    CHECK(rb_get(&rb, &b) == ERROR, "empty after draining");

    printf("\n=== 边界:size=2(有效容量 1)===\n");
    uint8_t small[2];
    RingBuf rb2;
    rb_init(&rb2, small, sizeof(small));
    CHECK(rb_put(&rb2, 0xAB) == SUCCESS, "put into size-2 buffer");
    CHECK(rb_put(&rb2, 0xCD) == ERROR, "second put fails (full)");
    CHECK(rb_get(&rb2, &b) == SUCCESS && b == 0xAB, "get from size-2 buffer");

    printf("\n=== 非法初始化 ===\n");
    RingBuf rb3;
    rb_init(&rb3, small, 3);   /* 3 不是 2 的幂 */
    CHECK(rb_put(&rb3, 0x01) == ERROR, "put into invalid buffer fails");
    CHECK(rb_available(&rb3) == SUCCESS, "available == 0 for invalid buffer");

    printf("\n=== 批量写/读 ===\n");
    uint8_t src1[] = {1,2,3,4,5};
    uint16_t w = rb_put_block(&rb, src1, sizeof(src1));
    CHECK(w == 5, "put_block writes 5");
    CHECK(rb_available(&rb) == 5, "available == 5");

    /* 批量读，部分读 */
    uint8_t dst[10] = {0};
    uint16_t r = rb_get_block(&rb, dst, 3);
    CHECK(r == 3, "get_block reads 3");
    CHECK(dst[0]==1 && dst[1]==2 && dst[2]==3, "get_block data correct");
    CHECK(rb_available(&rb) == 2, "available == 2 after partial get");

    /* 再读剩余 */
    r = rb_get_block(&rb, dst, 10);
    CHECK(r == 2, "get_block reads remaining 2");
    CHECK(dst[0]==4 && dst[1]==5, "remaining data 4,5");
    CHECK(rb_get_block(&rb, dst, 1) == 0, "get_block on empty returns 0");

    printf("\n=== 批量写截断（满）===\n");
    rb_reset(&rb);
    uint8_t big[20];
    for (int i = 0; i < 20; i++) big[i] = (uint8_t)i;
    w = rb_put_block(&rb, big, 20);          /* 有效容量 15，只能写 15 */
    CHECK(w == 15, "put_block truncates to 15");
    CHECK(rb_put_block(&rb, big, 1) == 0, "put_block on full returns 0");

    printf("\n=== 回绕批量写/读 ===\n");
    rb_reset(&rb);
    /* 先写 10，读 10，让 head/tail 都到 10 */
    rb_put_block(&rb, big, 10);
    rb_get_block(&rb, dst, 10);
    /* 现在 head=10, tail=10；再写 15 会回绕：先写 10..15（6 字节），再写 0..8（9 字节） */
    w = rb_put_block(&rb, big, 15);
    CHECK(w == 15, "put_block wraps and writes 15");
    CHECK(rb_available(&rb) == 15, "available == 15 after wrap write");

    uint8_t out[20];
    r = rb_get_block(&rb, out, 15);
    CHECK(r == 15, "get_block wraps and reads 15");
    /* 数据应是 big[0..14]（FIFO） */
    int ok = 1;
    for (int i = 0; i < 15; i++) if (out[i] != big[i]) ok = 0;
    CHECK(ok, "wrapped data order correct");

    printf("\n=== peek 不消费 ===\n");
    rb_reset(&rb);
    uint8_t p[] = {0xAA, 0xBB, 0xCC};
    rb_put_block(&rb, p, 3);

    uint8_t pk[3] = {0};
    uint16_t pk_n = rb_peek(&rb, pk, 3);
    CHECK(pk_n == 3, "peek returns 3");
    CHECK(pk[0]==0xAA && pk[1]==0xBB && pk[2]==0xCC, "peek data correct");
    CHECK(rb_available(&rb) == 3, "peek does not consume (available still 3)");

    /* 再 peek 一次，数据还在 */
    uint8_t pk2[3] = {0};
    CHECK(rb_peek(&rb, pk2, 3) == 3, "second peek still 3");
    CHECK(pk2[0]==0xAA, "second peek data still there");

    /* peek 截断 */
    CHECK(rb_peek(&rb, pk, 10) == 3, "peek truncates to available");

    /* 真正消费 */
    CHECK(rb_get_block(&rb, pk, 3) == 3, "get after peek");
    CHECK(rb_available(&rb) == 0, "empty after get");
    CHECK(rb_peek(&rb, pk, 3) == 0, "peek on empty returns 0");

    printf("\n=== peek 回绕 ===\n");
    rb_reset(&rb);
    rb_put_block(&rb, big, 10);
    rb_get_block(&rb, out, 8);               /* tail 到 8 */
    rb_put_block(&rb, big, 10);              /* 写回绕 */
    /* 此时数据从 tail=8 开始，可能回绕；peek 应能取到连续数据 */
    uint8_t pk3[10];
    uint16_t n3 = rb_peek(&rb, pk3, 10);
    CHECK(n3 == 10, "peek wraps and returns 10");
    /* 验证顺序：剩余旧数据 big[8..9]，然后新数据 big[0..7] */
    int ok2 = 1;
    for (int i = 0; i < 2; i++) if (pk3[i] != big[8+i]) ok2 = 0;
    for (int i = 0; i < 8; i++) if (pk3[2+i] != big[i]) ok2 = 0;
    CHECK(ok2, "peek wrapped data order correct");
    CHECK(rb_available(&rb) == 12, "peek wrapped does not consume");

    printf("\n%s\n", g_fail == 0 ? "ALL TESTS PASSED" : "SOME TESTS FAILED");
    return;
}

int main(void)
{
    rb_test_cases();
    return g_fail == 0 ? 0 : 1;
}
