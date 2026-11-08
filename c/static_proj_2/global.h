/* global.h */
#ifndef GLOBAL_H
#define GLOBAL_H

/* 声明一个全局变量，告诉编译器"它存在于某处，类型是 int"。
 * 注意：这里【不能】初始化，否则每个包含此头文件的 .c 都会
 * 产生一个定义，导致 "multiple definition" 链接错误。 */
extern int g_shared_counter;

/* 声明一个函数（同样只是声明） */
extern void g_increment(void);
extern int  g_get(void);

#endif /* GLOBAL_H */