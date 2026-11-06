/* arr.c :定义数组arr，并提供一个打印函数*/
#include <stdio.h>

//数组arr在.c文件中定义，存储在.data段中，提供一个打印函数print_arr()，打印数组arr的地址和内容
//故意设置arr[0],arr[1]的值，在64位小端机上，arr起始的8字节拼成的值为0x1111111122222222，方便在gdb中查看
int arr[10]={0x11111111,0x22222222,3,4,5,6,7,8,9,10};

void print_arr(void)
{
    printf("arr address: %p\n",(void*)arr);
    for(int i=0;i<10;i++)
    {
        printf("arr[%d]:0x%x\n",i,arr[i]);
    }
    return;
}//end of print_arr