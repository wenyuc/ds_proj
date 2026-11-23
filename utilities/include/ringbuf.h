/*
Ring Buffer Header

典型用法 RingBuf.buf存原始字节流，而flex_frame_c是协议层石图，把字节解释称帧头，
用memcpy或逐字节读len，再get_block取payload.代码在knowloege_base中ring_buffer.md
*/

#ifndef RINGBUF_H
#define RINGBUF_H

#include <stdint.h>
#include <stddef.h>
#include "ds_type.h"

/*
 * SPSC (Single Producer Single Customer) 环形缓冲区（留一个空位判满）
 * - 生产者只改 head，消费者只改 tail（SPSC 无锁的前提）
 * - 有效容量 = size - 1
 * - 元素类型为 uint8_t（字节流，UART/DMA 常用）
 */
typedef struct {
    uint8_t *buf;      /* 数据区，调用者提供（静态数组或 malloc） */
    uint16_t size;     /* 缓冲区总大小（必须是 2 的幂，便于 & 取模） */
    volatile uint16_t head;   /* 写索引，生产者修改 用voltile，防止编译器优化掉看似不变的读*/
    volatile uint16_t tail;   /* 读索引，消费者修改 */
} RingBuf;

/* 初始化：buf 由调用者提供，不内部malloc，符合嵌入式“静态分配”原则。
size 必须为 2 的幂（>=2），这样可以用(idx+1)&(size-1)代替%size ，避免除法（MCU上 % 开销大）
*/
Status rb_init(RingBuf *rb, uint8_t *buf, uint16_t size);

/* 生产者：写一个字节，成功返回 SUCCESS(0)，满返回 ERROR */
Status rb_put(RingBuf *rb, uint8_t byte);

/* 消费者：读一个字节，成功返回 SUCCESS，空返回 ERROR */
int rb_get(RingBuf *rb, uint8_t *byte);

/* 查询：当前可读字节数 */
uint16_t rb_available(const RingBuf *rb);

/* 查询：当前空闲空间（可写字节数） */
uint16_t rb_free(const RingBuf *rb);

/* 清空 */
Status rb_reset(RingBuf *rb);

/* 批量写：从 src 写 n 字节，返回实际写入字节数（0 表示满/失败） */
uint16_t rb_put_block(RingBuf *rb, const uint8_t *src, uint16_t n);

/* 批量读：读最多 n 字节到 dst，返回实际读出字节数（0 表示空/失败） */
uint16_t rb_get_block(RingBuf *rb, uint8_t *dst, uint16_t n);

/*
 * 查看不消费,返回dst[n]: 从 tail 开始看最多 n 字节，不消费（tail 不动）。
 * 返回实际可看字节数。
 * 若数据回绕，可能只返回第一段（tail..size-1）的长度；
 * 调用者可用 rb_peek_linear 分两段取，或用 rb_peek_copy 拷贝到连续缓冲。
 */
uint16_t rb_peek(const RingBuf *rb, uint8_t *dst, uint16_t n);

/*
 * 查看不消费，返回两段数据指针（零拷贝）。
 * - *len1：第一段长度（tail..min(size, tail+avail)-1）
 * - *ptr2：第二段指针（若未回绕则为 NULL）
 * - *len2：第二段长度（若未回绕则为 0）
 * 返回：总可看字节数（= *len1 + *len2），0 表示空/失败。
 * 注意：返回的指针在下一次 put/get 前有效；调用者不能修改其内容。
 */
uint16_t rb_peek_ptr(const RingBuf *rb,
                     const uint8_t **ptr1, uint16_t *len1,
                     const uint8_t **ptr2, uint16_t *len2);

#endif /* RINGBUF_H */