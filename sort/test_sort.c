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

//实现冒泡排序
Status bubbleSort(int *data, int len)
{
    //外层循环控制“趟数”，共需len-1趟
    for(int i=0; i<len; i++)
    {
        //内层循环控制每趟中相邻元素的比较和交换
        //每次比较到len-i-1，因为它的右侧已排好
        for(int j=0; j<len-i-1;j++)
        {
            if (data[j]>data[j+1])
            {
                swap(data, j, j+1);
            }
        }
    }
    return SUCCESS;
}//end of bubbleSort

//insertion sort
Status insertionSort(int *data, int len)
{
    for(int i=1; i<len; i++)
    {
        int key=data[i];
        int j=i-1;
        while(j>=0 && data[j]>key)
        {
            data[j+1]=data[j];
            j--;
        }
        data[j+1]=key;
    }
    return SUCCESS;
}//end of insertionSort

//binary insertion sort
Status binaryInsertionSort(int *data, int len)
{
    for(int i=1; i<len; i++)
    {
        int key=data[i];
        int low=0, high=i-1;
        while(low<=high)
        {
            int mid=(low+high)/2;
            if(data[mid]>key)
            {
                high=mid-1;
            }
            else
            {
                low=mid+1;
            }
        }
        for(int j=i-1; j>=low; j--)
        {
            data[j+1]=data[j];
        }
        data[low]=key;
    }
    return SUCCESS;
}//end of binaryInsertionSort

//简单选择排序
Status selectionSort(int *data, int len)
{
    for(int i=0; i<len-1; i++)
    {
        int minIndex=i;
        for(int j=i+1; j<len; j++)
        {
            if(data[j]<data[minIndex])
            {
                minIndex=j;
            }
        }
        if(minIndex!=i)
        {
            swap(data, i, minIndex);
        }
    }
    return SUCCESS;
}//end of selectionSort

//堆排序
Status heapSort1(int *data, int len)
{
    //建立最大堆
    for(int i=len/2-1; i>=0; i--)
    {
        int root=i;
        while(2*root+1<len)
        {
            int child=2*root+1;
            if(child+1<len && data[child]<data[child+1])
            {
                child++;
            }
            if(data[root]>=data[child])
            {
                break;
            }
            swap(data, root, child);
            root=child;
        }
    }
    //逐步提取最大元素
    for(int i=len-1; i>0; i--)
    {
        swap(data, 0, i);
        int root=0;
        while(2*root+1<i)
        {
            int child=2*root+1;
            if(child+1<i && data[child]<data[child+1])
            {
                child++;
            }
            if(data[root]>=data[child])
            {
                break;
            }
            swap(data, root, child);
            root=child;
        }
    }
    return SUCCESS;
}//end of heapSort1

//向下调整堆（从root开始，len为当前堆的大小）
Status adjust(int *data, int root, int len)
{
    while(2*root<=len)
    {
        int child=2*root;
        if(child+1<=len && data[child]<data[child+1])
        {
            child++;
        }
        if(data[root]>=data[child])
        {
            break;
        }
        //子结点child与父结点root交换，继续向下调整
        swap(data, root, child);
        root=child;
    }
    return SUCCESS;
}//end of adjust

Status heapSort(int *data, int len)
{
    //建堆，从最后一个非叶子结点开始向上调整
    for(int i=len/2; i>0; i--)
    {
        adjust(data, i, len);
    }
    
    //排序过程，将堆顶最大值依次交换到数组末尾，并调整剩余部分为最大堆
    for(int i=len; i>1; i--)
    {
        swap(data, 1, i);
        adjust(data, 1, i-1);
    }
    return SUCCESS;
}//end of heapSort

int main(void)
{
    int data[]={47,35,60,95,77,15,28};

    int len=sizeof(data)/sizeof(data[0]);

    bubbleSort(data,len);

    printf("排序后的数组: ");
    for (int i=0; i< len; i++)
    {
        printf("%d ",data[i]);
    }
    printf("\n");

    //插入排序
    int data2[]={47,35,60,95,77,15,28};
    int len2=sizeof(data2)/sizeof(data2[0]);
    insertionSort(data2, len2);

    printf("插入排序后的数组: ");
    for (int i=0; i< len2; i++)
    {
        printf("%d ",data2[i]);
    }
    printf("\n");

    //折半插入排序
    int data3[]={47,35,60,95,77,15,28};
    int len3=sizeof(data3)/sizeof(data3[0]);
    binaryInsertionSort(data3, len3);

    printf("折半插入排序后的数组: ");
    for (int i=0; i< len3; i++)
    {
        printf("%d ",data3[i]);
    }
    printf("\n");

    //简单选择排序
    int data4[]={47,35,60,95,77,15,28};
    int len4=sizeof(data4)/sizeof(data4[0]);
    selectionSort(data4, len4);

    printf("简单选择排序后的数组: ");
    for (int i=0; i< len4; i++)
    {
        printf("%d ",data4[i]);
    }
    printf("\n");

    //堆排序
    //int data5[]={47,35,60,95,77,15,28};
    //int len5=sizeof(data5)/sizeof(data5[0]);
    //heapSort(data5, len5);
    int data5[]={-1,47,35,60,95,77,15,28};
    int len5=sizeof(data5)/sizeof(data5[0]);
    heapSort(data5, len5-1);

    printf("堆排序后的数组: ");
    for (int i=1; i< len5; i++)
    {
        printf("%d ",data5[i]);
    }
    printf("\n");
    return SUCCESS;
}
