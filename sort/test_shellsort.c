#include <stdio.h>

#define SUCCESS 0
#define FAILURE 1
typedef int Status;

//交换数组中下表为m和n的两个元素的值
Status swap(int* data, int m, int n)
{
    int temp=data[m];
    data[m]=data[n];
    data[n]=temp;
    return SUCCESS;
}//end of swap

//实现希尔排序
Status shellSort(int *data, int len)
{
    for(int step=len/2; step>0; step/=2)
    {
        for(int i=step; i<len; i++)
        {
            int key=data[i];
            int j=i-step;
            while(j>=0 && data[j]>key)
            {
                data[j+step]=data[j];
                j-=step;
            }
            data[j+step]=key;
        }
    }
    return SUCCESS;
}//end of shellSort

int main(void)
{
    //待排序数组
    int data[]={47,35,60,95,77,15,28};
    int len=sizeof(data)/sizeof(data[0]);

    shellSort(data,len);

    printf("排序后的数组: ");
    for (int i=0; i< len; i++)
    {
        printf("%d ",data[i]);
    }
    printf("\n");
    return SUCCESS;
}
