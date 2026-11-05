#include <stdio.h>
#include <stdlib.h>

int func(int **temp) // 形参 temp 的类型必须是 int **，才能接住 &p
{
    *temp=(int*)malloc(sizeof(int)); //*temp就是main中的p，类型int *,在堆上分配一个整数大小的空间给*temp，即main中的p
    if(*temp == NULL) return -1;
    **temp=101;  //**temp就是main中的*p 
    return 0;
}

int func2(int *tmp) //形参tmp的类型必须是int *，才能接住q(类型int*).值传递，q指向的堆地址被拷贝给tmp,
                    //tmp和q指向同一块堆内存地址，改*tmp就改了那块内存地址中的内容。并没有改tmp和q说指的内存地址
{
    *tmp=102; //*tmp就是main中的*q. 另外，tmp是func2中的另一个变量，可以修改tmp使其指向另一块内存地址，并不影响q所指的内存地址
    return 0;
}
int main(void)
{
    int ret;
    //方式1
    int *p;    //声明，p为指针，指向保存整数的内存地址
    p=NULL;    //目前不指向任何有效的整数,所以p的内容为空，解引用解引用NULL为未定义行为。
    printf("&p : %p p: %p \n",(void*)&p, (void*)p); //指针变量p有自己的存储空间，在栈空间中。栈空间是在进入main时分配好的
    ret=func(&p);  //&p取的是“指针变量p自身的地址”（栈地址，类型int **）
               //&运算符的规则是“取谁的地址，结果类型就是‘指向谁的类型’". p 的类型是int *，所以&p的类型就是“指向int *的指针”，即int **
    if (ret == 0)
    {
        printf("&p: %p p: %p *p=%d\n",(void*)&p, (void*)p, *p);
    }

    //方式2
    int *q=(int *)malloc(sizeof(int));//声明q为指针，指向保存整数的内存地址，并在堆上分配一个保存整数大小的地址给q，指针q本身也在栈空间中分配了存储空间
    //malloc()只分配空间，不初始化，*q是为UB行为
    printf("&q: %p q: %p\n",(void *)&q, (void*)q); //指针本身的地址（栈空间），整数的地址（堆空间）
    ret=func2(q); //将整数存储空间的地址作为实参传给func2
    if (ret == 0)
    {
        printf("&q: %p q: %p *q: %d\n",(void*)&q, (void*)q ,*q); //指针本身的地址（栈空间），整数的地址（堆空间）都不变，整数值变了
    }
    
    free(q);
    free(p);  //p指向func中分配的内存，所以可以free
    return 0;
}