/*module_a.c*/
#include <stdio.h>

/*=== static 全局变量：只在module_a.c中可见 ===*/
static int secret_counter=0; //内部链接，其他文件看不到

/*=== 非static全局变量：其他文件可以用extern声明后访问 ===*/
int shared_counter=0; //外部链接

/*=== static函数，只在module_a.c中可见 ===*/
static void helper(void)
{
    secret_counter++;
    printf("[module_a] helper called, secret_counter=%d\n", secret_counter);
}

/*=== 非static函数，其他文件可以访问 ===*/
void increment_shared_counter(void)
{
    helper(); //本文件内可以调用static函数
    shared_counter++;
}

//通过该函数可以获取 static 变量的值
//这正是模块化 C 编程中"隐藏实现细节、只暴露接口"的基本手段
int get_secret(void)
{
    return secret_counter;
}

/*
$ gcc -c module_a.c
$ nm module_a.o|grep -E 'secret|helper|shared'
0000000000000055 T get_secret   'T'表示全局text(global)
0000000000000000 t helper       't'表示局部text(local)
0000000000000036 T increment_shared_counter
0000000000000004 b secret_counter  'b'表示本地BSS(local)
0000000000000000 B shared_counter  'B'表示全局BSS(global)
*/