/*
RINGBUF Implementation
*/

#include <string.h>
#include "ringbuf.h"

/* 判断 size 是否为 2 的幂,是2的幂返回1，否则返回0 */
static int is_pow2(uint16_t x)
{
    return (x != 0) && ((x & (x - 1)) == 0);
}//is_pow2

Status rb_init(RingBuf *rb, uint8_t *buf, uint16_t size)
{
    /* 调用者保证 size 是 2 的幂且 >= 2；这里做防御性检查 */
    if (rb == NULL || buf == NULL || size < 2 || !is_pow2(size)) {
        /* 非法参数：置为安全空状态（size=0 时所有操作都失败） */
        if (rb != NULL) {
            rb->buf = NULL;
            rb->size = 0;
            rb->head = 0;
            rb->tail = 0;
        }
        return ERROR;
    }
    rb->buf = buf;
    rb->size = size;
    rb->head = 0;
    rb->tail = 0;
    
    return SUCCESS;
}//rb_init

Status rb_put(RingBuf *rb, uint8_t byte)
{
    if (rb == NULL || rb->buf == NULL) return -1;

    uint16_t next = (uint16_t)((rb->head + 1) & (rb->size - 1));
    if (next == rb->tail) {
        return ERROR;              /* 满（留一个空位） */
    }
    rb->buf[rb->head] = byte;
    rb->head = next;            /* 先写数据, 再更新 head(顺序重要),消费者看到head更新时，数据已就位 */
    return SUCCESS;
}//rb_put

Status rb_get(RingBuf *rb, uint8_t *byte)
{
    if (rb == NULL || rb->buf == NULL || byte == NULL) return ERROR;

    if (rb->tail == rb->head) {
        return ERROR;              /* 空 */
    }
    *byte = rb->buf[rb->tail];
    rb->tail = (uint16_t)((rb->tail + 1) & (rb->size - 1));  /* 先读数据，再更新tail，生产者看到tail更新时，空间已释放 */
    return SUCCESS;
}//rb_get

uint16_t rb_available(const RingBuf *rb)
{
    if (rb == NULL || rb->buf == NULL) return 0;
    return (uint16_t)((rb->head - rb->tail) & (rb->size - 1));  //用&(size-1)处理索引回绕的减法
}//rb_available

uint16_t rb_free(const RingBuf *rb)
{
    if (rb == NULL || rb->buf == NULL) return 0;
    /* 空闲 = 总容量 - 1 - 已用（留一个空位） */
    return (uint16_t)(rb->size - 1 - rb_available(rb));
}//rb_free

Status rb_reset(RingBuf *rb)
{
    if (rb == NULL) return ERROR;
    rb->head = 0;
    rb->tail = 0;
    return SUCCESS;
}//rb_reset

/* ---------- 批量写 ---------- */
uint16_t rb_put_block(RingBuf *rb, const uint8_t *src, uint16_t n)
{
    if (rb == NULL || rb->buf == NULL || src == NULL || n == 0) return 0;

    uint16_t free_space = rb_free(rb);      /* 可用空间（留一个空位后） */
    if (n > free_space) n = free_space;     /* 最多写这么多 */
    if (n == 0) return 0;

    uint16_t head = rb->head;
    uint16_t size = rb->size;

    /* 第一段：从 head 到缓冲区末尾 */
    uint16_t first = (uint16_t)(size - head);
    if (first > n) first = n;
    memcpy(&rb->buf[head], src, first);

    /* 第二段：若回绕，从 0 开始写剩余 */
    uint16_t second = (uint16_t)(n - first);
    if (second > 0) {
        memcpy(&rb->buf[0], src + first, second);
    }

    rb->head = (uint16_t)((head + n) & (size - 1));
    return n;
}//rb_put_block

/* ---------- 批量读 ---------- */
uint16_t rb_get_block(RingBuf *rb, uint8_t *dst, uint16_t n)
{
    if (rb == NULL || rb->buf == NULL || dst == NULL || n == 0) return 0;

    uint16_t avail = rb_available(rb);
    if (n > avail) n = avail;               /* 最多读这么多 */
    if (n == 0) return 0;

    uint16_t tail = rb->tail;
    uint16_t size = rb->size;

    /* 第一段：从 tail 到缓冲区末尾 */
    uint16_t first = (uint16_t)(size - tail);
    if (first > n) first = n;
    memcpy(dst, &rb->buf[tail], first);

    /* 第二段：若回绕，从 0 开始读剩余 */
    uint16_t second = (uint16_t)(n - first);
    if (second > 0) {
        memcpy(dst + first, &rb->buf[0], second);
    }

    rb->tail = (uint16_t)((tail + n) & (size - 1));
    return n;
}//rb_get_block

/* ---------- 查看（不消费） 返回dst[n]---------- */
uint16_t rb_peek(const RingBuf *rb, uint8_t *dst, uint16_t n)
{
    if (rb == NULL || rb->buf == NULL || dst == NULL || n == 0) return 0;

    uint16_t avail = rb_available(rb);
    if (n > avail) n = avail;
    if (n == 0) return 0;

    uint16_t tail = rb->tail;
    uint16_t size = rb->size;

    uint16_t first = (uint16_t)(size - tail);
    if (first > n) first = n;
    memcpy(dst, &rb->buf[tail], first);

    uint16_t second = (uint16_t)(n - first);
    if (second > 0) {
        memcpy(dst + first, &rb->buf[0], second);
    }

    /* 注意：不修改 rb->tail */
    return n;
}//rb_peek

/* 查看不消费，返回2个指针及相应字节长度
*ptr1指向第一段数据的首字节，*len1是第一段数据的字节数
*ptr2指向第二段数据的首字节，*len2是第二段数据的字节数
*/
uint16_t rb_peek_ptr(const RingBuf *rb,
                     const uint8_t **ptr1, uint16_t *len1,
                     const uint8_t **ptr2, uint16_t *len2)
{
    /* 防御：输出参数必须有效 */
    if (ptr1 == NULL || len1 == NULL || ptr2 == NULL || len2 == NULL) {
        return 0;
    }
    *ptr1 = NULL; *len1 = 0;
    *ptr2 = NULL; *len2 = 0;

    if (rb == NULL || rb->buf == NULL) return 0;

    uint16_t avail = rb_available(rb);   /* 可读总字节数 */
    if (avail == 0) return 0;

    uint16_t tail = rb->tail;
    uint16_t size = rb->size;

    /* 第一段：从 tail 到缓冲区末尾，但不能超过 avail */
    uint16_t first = (uint16_t)(size - tail);
    if (first > avail) first = avail;

    *ptr1 = &rb->buf[tail];
    *len1 = first;

    /* 第二段：若还有剩余（说明回绕了），从 buf[0] 开始 */
    uint16_t second = (uint16_t)(avail - first);
    if (second > 0) {
        *ptr2 = &rb->buf[0];
        *len2 = second;
    }

    return avail;
}