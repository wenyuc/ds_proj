/* main.c 
gcc -Wall -Wextra -o bin/static_demo2 global.c main.c other.c
*/
#include <stdio.h>
#include "global.h"   /* 拿到 g_shared_counter 的 extern 声明 */

int main(void)
{
    printf("[main] initial g_shared_counter = %d\n", g_shared_counter);

    g_increment();
    g_increment();

    printf("[main] after 2 increments = %d\n", g_shared_counter);

    extern void other_print(void);   /* 另一个使用者里的函数 */
    other_print();

    return 0;
}