/*快速排序的实现*/
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

// partition 函数用于将数组划分为左右两部分，左边部分的元素都小于等于基准元素，
// 右边部分的元素都大于基准元素，返回基准元素的位置
int partition(int *data, int low, int high){
    int pivot=data[low]; //选择第一个元素(左端元素)作为基准元素
    
    //当low与high没有相遇时，继续循环
    while(low<high)
    {
        while(low<high && data[high]>=pivot)
            high--;
        data[low]=data[high]; //将右边小于基准的元素移到左边

        while(low<high && data[low]<=pivot)
            low++;
        data[high]=data[low]; //将左边大于基准的元素移到右边
    }

    //当low=high时，即找到pivot应该放的位置
    data[low]=pivot; //将基准元素放到正确的位置
    return low; //返回基准元素的位置
}//end of partition

//partition的另一个实现
int partition1(int *data, int low, int high)
{
    int pivot=data[low]; //选择第一个元素(左端元素)作为基准元素
    int i=low, j=high;
    while(i<j)
    {
        while(i<j && data[j]>=pivot)
            j--;
        while(i<j && data[i]<=pivot)
            i++;
        if(i<j)
            swap(data, i, j);
    }
    swap(data, low, i); //将基准元素放到正确的位置
    return i; //返回基准元素的位置
}

//快速排序的实现，采用递归的方式
Status quickSort(int *data, int left, int right)
{
    if(left>=right)
        return SUCCESS;

    int pivotIndex=partition(data, left, right);
    quickSort(data, left, pivotIndex-1);
    quickSort(data, pivotIndex+1, right);

    return SUCCESS;
}
int main(void)
{
    //待排序数组
    int data[]={47,35,60,95,77,15,28};
    int len=sizeof(data)/sizeof(data[0]);

    quickSort(data,0,len-1);

    printf("排序后的数组: ");
    for (int i=0; i< len; i++)
    {
        printf("%d ",data[i]);
    }
    printf("\n");
    return SUCCESS;
}
