/*
Unit test FlexFrame
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "flex_arr.h"

int main(void)
{
    printf("sizeof(struct uart_frame) = %zu\n", sizeof(struct uart_frame));  /* 4 */

    /* ---- 1. 创建帧 ---- */
    uint8_t data[] = {0x01, 0x02, 0x03, 0x04, 0x05};
    struct uart_frame *f = create_uart_frame(0x10, data, sizeof(data));
    if (f == NULL) return 1;

    printf("创建: ");
    dump_uart_frame(f);

    /* ---- 2. 模拟发送：把帧序列化成字节流 ---- */
    size_t total = sizeof(struct uart_frame) + f->len;
    uint8_t *wire = malloc(total);
    if (wire == NULL) { free(f); return 1; }

    wire[0] = f->sof;
    wire[1] = f->cmd;
    wire[2] = (uint8_t)(f->len & 0xFF);         /* 小端 */
    wire[3] = (uint8_t)(f->len >> 8);
    memcpy(wire + 4, f->payload, f->len);

    printf("发送字节流 (%zu 字节): ", total);
    for (size_t i = 0; i < total; i++)
        printf("%02X ", wire[i]);
    printf("\n");

    /* ---- 3. 模拟接收：从字节流解析 ---- */
    struct uart_frame *r = parse_uart_frame(wire, total);
    if (r == NULL) { free(wire); free(f); return 1; }

    printf("解析: ");
    dump_uart_frame(r);

    /* ---- 4. 释放 ---- */
    free(f);
    free(wire);
    free(r);
    return 0;
}