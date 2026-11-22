/*
flex arr implementation
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "flex_arr.h"

struct uart_frame *create_uart_frame(uint8_t cmd, const uint8_t *payload, uint16_t len)
{
    struct uart_frame *f = malloc(sizeof(*f) + (size_t)len);
    if (f == NULL) return NULL;
    f->sof = 0xAA;
    f->cmd = cmd;
    f->len = len;
    if (len > 0 && payload != NULL)
        memcpy(f->payload, payload, len);
    return f;
}

void dump_uart_frame(const struct uart_frame *f)
{
    printf("sof=0x%02X cmd=0x%02X len=%u payload=[", f->sof, f->cmd, f->len);
    for (uint16_t i = 0; i < f->len; i++)
        printf("%02X%s", f->payload[i], (i + 1 < f->len) ? " " : "");
    printf("]\n");
}

struct uart_frame *parse_uart_frame(const uint8_t *buf, size_t buflen)
{
    if (buf == NULL || buflen < 4) return NULL;   /* 头部至少 4 字节 */

    uint8_t  sof = buf[0];
    uint8_t  cmd = buf[1];
    uint16_t len = (uint16_t)buf[2] | ((uint16_t)buf[3] << 8);  /* 小端 */

    if (sof != 0xAA) return NULL;                  /* 帧头校验 */
    size_t total = 4 + (size_t)len;
    if (buflen < total) return NULL;               /* 数据不够 */

    struct uart_frame *f = malloc(total);
    if (f == NULL) return NULL;
    f->sof = sof;
    f->cmd = cmd;
    f->len = len;
    memcpy(f->payload, buf + 4, len);
    return f;
}

