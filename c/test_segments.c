/*
分析各类变量所在的内存区域
project: knowledge_base/程序的内存区.md
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

int global_var = 10; // 已初始化全局变量，存储在数据段（可读写）
int global_uninit_var; // 未初始化全局变量，存储在BSS段（可读写）
static int static_var = 20; // 已初始化静态变量，存储在数据段（可读写）
static int static_uninit_var; // 未初始化静态变量，存储在BSS段（可读写）
const int const_var = 30; // 常量，存储在只读数据段(.rodata段)（只读）
const int const_uninit_var; // 未初始化常量，存储在BSS段（只读）,但是无用的声明，因为后序不能赋值(因为是const类型),也不能读(UB)

int main(void)
{
    //栈区	
	int 	 mq1;			//未初始化局部变量
	int  	*mq2;			//未初始化局部指针变量

    int   	mq3=3;		     //已初始化局部变量	
	char 	qq[10] = "hello"; //已初始化局部数组

    const  	int   mq4;	//未初始化局部只读变量，但是无用的声明，因为后序不能赋值(因为是const类型),也不能读(UB)
	const   int   mq5=3;//已初始化局部只读变量

    //堆区
	const int *p1 = malloc(4); //已初始化局部指针变量p1, p1是一个指向常量的指针，指针本身可以修改，但指针指向的内存不能修改
	int * const p2 = malloc(4); //已初始化局部指针变量p2,p2是一个常量指针，指向的内存可以修改，但指针本身不能修改		

    //全局(静态)区
	static 	int mp1;		//未初始化局部静态变量	
	static  int mp2=2;		//已初始化局部静态变量

    //常量区
	char *vv = "I LOVE YOU";//已初始化局部指针变量,在.rodata段中存储字符串常量"I LOVE YOU"，vv指向这块只读内存
	char *mq = "5201314";

    printf("\n栈区-变量地址\n");
	printf("未初始化局部变量 mq1		:0x%p\r\n",(void *)&mq1);
    mq1=1;//可修改未初始化的局部变量mq1的值
	printf("未初始化局部指针变量 mq2		:0x%p\r\n",(void *)&mq2);
    mq2=&mq1;//可修改未初始化的局部指针变量mq2的值
	printf("已初始化局部变量 mq3			:0x%p\r\n",(void *)&mq3);
    mq3=mq1+2;//可修改已初始化的局部变量mq3的值
	printf("已初始化局部数组 qq			:0x%p\r\n", (void *)qq );
    qq[0]='H';//可修改已初始化的局部数组qq的值
    printf("已初始化局部数组修改后值 qq	: %s\r\n", qq);

    printf("未初始化局部只读变量 mq4		:0x%p\r\n",(void *)&mq4);
    // mq4=10;//const对象禁止一切赋值，未初始化只是让它更无解
	printf("已初始化局部只读变量 mq5		:0x%p\r\n",(void *)&mq5);
    // mq5=20;//const对象禁止一切赋值

    //但是下面四行却是正确的。因为第59行const是修饰所指的内容，而不是指针本身
    //所以 第61行赋值正确。
    char *chPtr="hello";
    const char *cchPtr;
    cchPtr = chPtr;
    printf("main: %s\n", cchPtr);

    printf("\n堆区-动态申请地址\r\n");
	printf("已初始化局部int型指针变量p1   :0x%p\r\n", (void *)p1);
	printf("已初始化局部int型指针变量p2   :0x%p\r\n", (void *)p2);

    printf("\n全局区-变量地址\n");
	printf("未初始化全局变量 	:%p\r\n",(void *)&global_uninit_var);
    global_uninit_var=1;//可修改未初始化的全局变量global_uninit_var的值
	printf("未初始化静态变量		:%p\r\n",(void *)&static_uninit_var);
    static_uninit_var=10;//可修改未初始化的静态变量static_uninit_var的值
	printf("未初始化只读变量		:%p\r\n",(void *)&const_uninit_var);
    //const_uninit_var=20;//不可修改未初始化的只读变量const_uninit_var的值

    printf("已初始化全局变量 	:%p\r\n",(void *)&global_var);
    global_var=2;//可修改已初始化的全局变量global_var的值
	printf("已初始化静态变量		:%p\r\n",(void *)&static_var);
    static_var=20;//可修改已初始化的静态变量static_var的值

    printf("未初始化局部静态变量	 :%p\r\n",(void *)&mp1);
    mp1=1;//可修改未初始化的局部静态变量mp1的值
	printf("已初始化局部静态变量	 :%p\r\n",(void *)&mp2);	
    mp2=mp1+2;//可修改已初始化的局部静态变量mp2的值		

    printf("\n常量区地址\n");
	printf("已初始化只读变量		  :%p\r\n",(void *)&const_var);
	printf("已初始化局部指针变量	  :%p\r\n", (void *)vv );
    //strcpy(vv,"PEACE"); //编译时通过，但运行时会报错，段错误（Segmentation fault），因为vv指向的内存是只读的
	printf("已初始化局部指针变量	  :%p\r\n", (void *)mq );
    /*
    关键区分：char * vs char[]
    char *vv = "I LOVE YOU";     // ❌ vv 指向只读字面量，不能改内容
    char vv[] = "I LOVE YOU";    // ✅ vv 是数组，内容在栈上，可改
    strcpy(vv, "PEACE");         // ✅ 写入栈数组，合法
    但注意：vv[] 数组大小固定为 11 字节，"PEACE" 只有 6 字节，放得下；
    若目标串比数组长会缓冲区溢出，所以更安全的是 strncpy / snprintf / strcpy_s。
    */

    printf("\n代码区地址\n");
    // 先 main → uintptr_t（整数），再 uintptr_t → void *。
    // 两步都是"整数↔指针"，-Wpedantic 对"整数转指针"通常不报（C 标准允许实现定义）。这是最实用的写法：
	printf("main entry: %p\n", (void *)(uintptr_t)main);

    free((void*)p1);
    free((void*)p2);

    return 0;
}