/*test_arr.c
引用arr.c中定义的数组arr和函数print_arr()，并在main()中打印数组arr的地址和内容
编译链接: gcc -Wall -Wextra -o bin/test_arr arr_extern.c test_arr.c
*/
#include <stdio.h>
#include <string.h>

//extern int *arr; //错误，类型不匹配，arr.c中的arr是int[10],这里声明为int *，编译链接都成功，因为名字匹配即可
//链接器只匹配符号名，类型一致性靠程序员保证：extern 声明的类型必须和定义完全一致（数组 vs 指针不能混）
//"数组名就是指针"，这只在"表达式中的数组名退化为指向首元素的指针"这一语境下成立
//符号本身的类型：数组就是数组，不是指针。extern 声明必须尊重这一点。
extern int arr[]; //正确，类型匹配，arr.c中的arr是int[10],这里声明为int arr[]，
void print_arr(void); //声明print_arr()函数，函数定义在arr.c中

int main(void)
{
    printf("---例1: 定义字符串常量与字符串初始化为数组的区别---\n");
    char a[]="abcdefgh"; //定义数组a时分配了9个字节的内存空间(包括最后一个字符\0),内存空间在栈上分配
    char *p=a; //定义指针变量p，指向数组a的首元素a[0]，指针变量p在栈上分配内存空间，变量p内存空间为8字节
    char *q="opqrstuv"; //定义指针变量q，指向字符串常量"opqrstuv"的首元素q[0]，字符串常量在程序的只读数据区分配内存空间，
                        //指针变量q在栈上分配内存空间，指针变量q内存空间为8字节。不能通过指针q修改字符串常量的内容，否则会出现段错误
    int size_a=sizeof(a)/sizeof(a[0]);
    strncpy(a,"ijklm",size_a-1); //字符串常量初始化的数组时可以修改
    
    printf("&p=%p &q=%p\n",(void*)&p,(void*)&q); //打印指针变量p和q的地址
    for(int i=0; i<size_a; i++)
    {
        printf("a[%d]:%c\n",i, a[i]); //最后一个元素是'\0'，所以打印出来是空白
    }
    printf("arr[0]: %d arr[1]: %d\n", (*p3)[0],(*p3)[1]); //打印arr数组的前两个元素,先解引用得到数组，再下标

    printf("---例2: 三种指向数组arr的指针类型---\n");
    int arr_i[10]={1,2,3,4,5,6,7,8,9,10}; //定义数组arr_i，存储在栈上
    int *p1=arr_i; //定义指针变量p1，arr_i退化为指针，指向数组arr_i的首元素arr_i[0],指针变量也在栈上
    int *p2=&arr_i[0]; //定义指针变量p2，指向数组arr_i的首元素arr_i[0],指针变量也在栈上,同上
    int (*p3)[10]=&arr_i; //定义指针变量p3，p3 is a pointer to an array of 10 integers，所以它指向整个数组arr_i
    //arr_i, &arr_i[0], &arr_i都是指向数组首元素的地址，类型不同，这是关键。
    //&arr是指向整个数组的指针，类型是int (*)[10]，而arr和&arr[0]是指向首元素的指针，类型是int *
    
    printf("arr_i[0]: %d arr_i[1]: %d\n",(*p3)[0],(*p3)[1]); //先解引用再下标
    printf("arr_i[9]: %d\n", p3[0][9]); //也可以
    p3=p3+1; //指针p3指向下一个数组arr_i[10]，即arr_i的后面10个整数的地址
    printf("p3_i+1: %p\n",(void*)p3); //打印p3指向的地址，应该是arr_i的后面10个整数的地址,但此时(*p3)[0]是未定义行为
    
    // char (*p3)[10]最典型的用途是接收二维数组的某一行.
    //TODO: 这里可以加一个二维数组的例子，说明二维数组的行是一个一维数组，指向一维数组的指针类型就是char (*)[10]，而不是char *。

    printf("---例3: 引用外部数组时，不能把数组当指针使用---\n");
    printf("test_arr: 把 arr 当指针, arr 的值 = %p\n", (void *)arr);
    printf("test_arr: 即将执行 arr[0] = 999 ...\n");
    fflush(stdout);

    //假设 arr 的地址是 0x1000：
    //arr.c：arr 是int数组，arr[0] 在 0x1000，arr[1] 在 0x1004……
    //test_arr.c：arr 被当指针，arr 的"值"= 读 0x1000 处的 8 字节 = arr[0] 和 arr[1] 拼成的数（若都是 0，则 arr == 0x0，即 NULL）。
    //然后 arr[0] = 1 等价于 *(NULL + 0) = 1 → 段错误。
    //即使不是 NULL，那个值也是无意义的"数据被当地址"。

    arr[0] = 999;   /* 等价于 *(arr + 0) = 999，arr 是那个非法地址 → 崩溃 */

    printf("test_arr: 没崩溃?arr[0] = %d\n", arr[0]);
    print_arr();    
    return 0;
}//end of main