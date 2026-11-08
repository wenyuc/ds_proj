/* global.c —— 全局变量和函数的【定义】放这里，且只放一份 */
#include "global.h" //让编译器核对"定义的类型和头文件声明一致"（不一致会报错，这是额外的好处）。

/* 定义（definition）：这里才真正分配存储空间。
 * 不写 extern，写 extern 且带初始化也行，但通常不写。 */
int g_shared_counter = 0;

void g_increment(void)
{
    g_shared_counter++;
}

int g_get(void)
{
    return g_shared_counter;
}