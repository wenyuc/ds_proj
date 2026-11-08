/* other.c —— 另一个使用 g_shared_counter 的源文件 */
#include <stdio.h>
#include "global.h"   /* 同样拿到 extern 声明 */

void other_print(void)
{
    printf("[other] sees g_shared_counter = %d\n", g_shared_counter);

    /* 也可以直接改它（main.c 和 other.c 操作的是同一块内存（global.c 里定义的那一个 int），所以修改互相可见。） */
    g_shared_counter += 10;
    printf("[other] after += 10, g_shared_counter = %d\n", g_shared_counter);
}