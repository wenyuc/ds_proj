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

//合并两个有序子数组
Status merge(int *data, int left, int mid, int right)
{
    int temp[right-left+1]; //临时数组用于存储合并后的结果
    int i=left, j=mid+1, k=0; //k为临时数组下标
    while(i<=mid && j<=right)
    {
        if(data[i]<=data[j])
            temp[k++]=data[i++];
        else
            temp[k++]=data[j++];
    }
    //将剩余的左边部分拷贝到temp中（如果有） 
    while(i<=mid)
        temp[k++]=data[i++];
    //将剩余的右边部分拷贝到temp中（如果有）
    while(j<=right)
        temp[k++]=data[j++];
    //将临时数组中的元素拷贝回原数组
    for(int i=left; i<=right; i++)
        data[i]=temp[i-left];

    return SUCCESS;
}//end of merge

//合并两个有序子数组的另一种实现
Status merge1(int *data, int left, int mid, int right)
{
    int i=left, j=mid+1, k=left;
    int temp[right-left+1];
    while(i<=mid && j<=right)
    {
        if(data[i]<=data[j])
            temp[k-left]=data[i++];
        else
            temp[k-left]=data[j++];
        k++;
    }
    while(i<=mid)
        temp[k++-left]=data[i++];
    while(j<=right)
        temp[k++-left]=data[j++];

    for(int i=left; i<=right; i++)
        data[i]=temp[i-left];

    return SUCCESS;
}//end of merge

//实现归并排序
Status mergeSort(int *data, int left, int right)
{
    if(left>=right)
        return SUCCESS;

    int mid=(left+right)/2;
    mergeSort(data, left, mid);
    mergeSort(data, mid+1, right);

    //将排好序的左右两部分合并
    merge(data, left, mid, right);

    return SUCCESS;
}//end of mergeSort

//归并排序的另一种实现
Status mergeSort1(int *data, int len)
{
    if(len<=1)
        return SUCCESS;

    int mid=len/2;
    int left[mid];
    int right[len-mid];

    for(int i=0; i<mid; i++)
        left[i]=data[i];
    for(int i=mid; i<len; i++)
        right[i-mid]=data[i];

    mergeSort1(left, mid);
    mergeSort1(right, len-mid);

    int i=0, j=0, k=0;
    while(i<mid && j<len-mid)
    {
        if(left[i]<=right[j])
            data[k++]=left[i++];
        else
            data[k++]=right[j++];
    }
    while(i<mid)
        data[k++]=left[i++];
    while(j<len-mid)
        data[k++]=right[j++];

    return SUCCESS;
}//end of mergeSort1

int main(void)
{
    //待排序数组
    int data[]={47,35,60,95,77,15,28};
    int len=sizeof(data)/sizeof(data[0]);

    mergeSort(data,0,len-1);
    //另一种实现
    //mergeSort1(data,len);

    printf("排序后的数组: ");
    for (int i=0; i< len; i++)
    {
        printf("%d ",data[i]);
    }
    printf("\n");
    return SUCCESS;
}
