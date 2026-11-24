/*
Unit test FlexFrame
*/

#include <stdlib.h>
#include <string.h>
#include "ds_type.h"
#include "flex_arr.h"

int main(void)
{
    CHECK(sizeof(struct uart_frame) == 4, "uart_frame header size");

    /* ---- 1. 创建帧 ---- */
    uint8_t data[] = {0x01, 0x02, 0x03, 0x04, 0x05};
    struct uart_frame *f = create_uart_frame(0x10, data, sizeof(data));
    CHECK(f != NULL, "create_uart_frame returns a frame");
    if (f == NULL) return 1;
    CHECK(f->sof == 0xAA, "frame start byte");
    CHECK(f->cmd == 0x10, "frame command");
    CHECK(f->len == sizeof(data), "frame payload length");
    CHECK(memcmp(f->payload, data, sizeof(data)) == 0, "frame payload");

    /* ---- 2. 模拟发送：把帧序列化成字节流 ---- */
    size_t total = sizeof(struct uart_frame) + f->len;
    uint8_t *wire = malloc(total);
    CHECK(wire != NULL, "allocate wire buffer");
    if (wire == NULL) { free(f); return 1; }

    wire[0] = f->sof;
    wire[1] = f->cmd;
    wire[2] = (uint8_t)(f->len & 0xFF);         /* 小端 */
    wire[3] = (uint8_t)(f->len >> 8);
    memcpy(wire + 4, f->payload, f->len);
    CHECK(wire[0] == f->sof && wire[1] == f->cmd, "serialized frame header");
    CHECK(wire[2] == (uint8_t)(f->len & 0xFF) &&
          wire[3] == (uint8_t)(f->len >> 8), "serialized payload length");
    CHECK(memcmp(wire + sizeof(struct uart_frame), f->payload, f->len) == 0,
          "serialized payload");

    /* ---- 3. 模拟接收：从字节流解析 ---- */
    struct uart_frame *r = parse_uart_frame(wire, total);
    CHECK(r != NULL, "parse serialized frame");
    if (r == NULL) { free(wire); free(f); return 1; }
    CHECK(r->sof == f->sof, "parsed start byte");
    CHECK(r->cmd == f->cmd, "parsed command");
    CHECK(r->len == f->len, "parsed payload length");
    CHECK(memcmp(r->payload, f->payload, f->len) == 0, "parsed payload");

    /* ---- 4. 释放 ---- */
    free(f);
    free(wire);
    free(r);
    return g_fail == 0 ? 0 : 1;
}
