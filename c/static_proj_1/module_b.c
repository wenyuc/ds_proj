/* module_b.c 
gcc -Wall -Wextra -o bin/static_demo1 module_a.c module_b.c
*/
#include <stdio.h>

/* 访问 module_a.c 的"非 static"全局变量 —— 合法 
extern 解决的不是编译问题，而是跨文件的符号链接匹配问题.
*/
extern int shared_counter;

/* 访问 module_a.c 的"非 static"函数 —— 合法 */
void increment_shared_counter(void);
int  get_secret(void);

/* ===== 错误示范（编译/链接会报错，先注释掉） ===== */
// extern int secret_counter;   // ❌ secret_counter 是 static，其他文件链接不到
// extern void helper(void);    // ❌ helper 是 static，其他文件链接不到

/* module_b 自己也可以有一个同名的 static 变量，互不冲突 */
static int secret_counter = 999;   /* 与 module_a 的 static 变量完全独立 */

int main(void)
{
    printf("[module_b] shared_counter = %d\n", shared_counter);
    printf("[module_b] 自己的 static secret_counter = %d\n", secret_counter);

    increment_shared_counter();
    increment_shared_counter();

    printf("[module_b] after 2 increments:\n");
    printf("  shared_counter = %d\n", shared_counter);
    printf("  module_a 的 secret_counter = %d\n", get_secret());
    printf("  module_b 的 secret_counter = %d\n", secret_counter);  /* 仍是 999 */

    return 0;
}//end of main