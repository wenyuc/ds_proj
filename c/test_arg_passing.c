/*
分析形参和实参的参数传递
来自“Expert C Programming: Deep C Secrets” chapter 1, p19-20
*/

#include <stdio.h>
int foo(const char **p)
{
    printf("foo: %s\n", *p);
    //*p = "world";
    return 2;
}//end of foo

int main(int argc, char **argv)
{
    /*
    ANSI C标准描述了赋值的约束条件:
    要使赋值形式合法，必须满足以下条件之一:
    两个操作数都是指向有限定符或无限定符的相容类型的指针，左边指针所这指向的类型必须具有右边指针所指向的类型的所有限定符。
    也就是说，左边指针所指向的类型必须是右边指针所指向的类型的一个超集。
    例如:
    #23-26是合法的，因为左边指针所指向的类型是有const限定符的char的指针，
    而右边指针所指向的类型是没有const限定符的char的指针，且char类型与char类型是相容的。
    左边的指针所指向的类型是右边指针所指向的类型的一个超集。
    */
    char *chPtr="hello";
    const char *cchPtr;
    cchPtr = chPtr;
    printf("main: %s\n", cchPtr);

    /*
    而下例子是非法的。
    简单说:foo函数的参数是一个指向const char类型的指针的指针，而main函数中传入的是一个指向char类型的指针的指针。
    形参 const char ** 是一个没有限定符的指针类型，它的类型是“指向有‘const限定符的char类型’的指针的指针”，
    而实参 char ** 也是一个没有限定符的指针类型，它的类型是“指向没有限定符的char类型的指针的指针”。
    但它们说指向的类型不一样，形参指向const char*，而实参指向char*，所以它们不是相容类型。
    */
    int ret;
    //ret = foo(argv);  //错误，解释见上注释
    ret=foo((const char**)argv);
    printf("main: %d %s ret=%d\n", argc, argv[0], ret);
    return 0;
}