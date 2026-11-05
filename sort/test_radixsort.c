/*基数排序的实现*/
#include <stdio.h>

#define SUCCESS 0
#define FAILURE 1
#define MAX 100
#define RADIX 10 //十进制，桶的数量为10 （0-9）
typedef int Status;

//获取某个数在某一位(pos)上的数字,从右往左数，个位为第1位，pos=1
int getDigit(int num, int pos)
{
    int exp=1;
    for(int i=1; i<pos; i++)
        exp*=10;
    return (num / exp) % 10;
}//end of getDigit

//获取数组中最大数的位数
int getMaxDigits(int *data, int len)
{
    int max=data[0];
    for(int i=0; i<len; i++)
    {
        if(data[i]>max)
            max=data[i];
    }
    int digits=0;
    while(max>0)
    {
        max/=10;
        digits++;
    }
    return digits;
}//end of getMaxDigits

//基数排序的实现
Status radixSort(int *data, int len)
{
    int maxDigits=getMaxDigits(data,len); //获取最大位数，决定排序轮数
    int bucket[RADIX][len]; //每个桶最多存放len个数
    int bucketCount[RADIX]={0}; //记录每个桶的元素个数

    for(int pos=1; pos<=maxDigits; pos++) //从个位数开始，依次对每一位进行排序
    {
        //将元素放入对应的桶中
        for(int i=0; i<len; i++)
        {
            int digit=getDigit(data[i],pos); //获取当前元素在指定位上的数字
            bucket[digit][bucketCount[digit]++]=data[i]; //放入对应桶中
        }

        //收集阶段，将桶中的元素依次放回原数组
        int index=0;
        for(int i=0; i<RADIX; i++)
        {
            for(int j=0; j<bucketCount[i]; j++)
            {
                data[index++]=bucket[i][j];
            }
            bucketCount[i]=0; //重置桶的元素个数
        }
    }
    return SUCCESS;
}//end of radixSort

//基数排序的另一种实现
Status radixSort1(int *data, int len)
{
    int max=0;
    for(int i=0; i<len; i++)
    {
        if(data[i]>max)
            max=data[i];
    }

    for(int exp=1; max/exp>0; exp*=10)
    {
        int output[len];
        int count[10]={0};

        for(int i=0; i<len; i++)
        {
            int digit=(data[i]/exp)%10;
            count[digit]++;
        }

        for(int i=1; i<10; i++)
        {
            count[i]+=count[i-1];
        }

        for(int i=len-1; i>=0; i--)
        {
            int digit=(data[i]/exp)%10;
            output[--count[digit]]=data[i];
        }

        for(int i=0; i<len; i++)
        {
            data[i]=output[i];
        }
    }
    return SUCCESS;
}//end of radixSort

int main(void)
{
    //待排序数组
    int data[]={147,35,2360,995,377,25,128,8,1465,2311,465,89,10,188};
    int len=sizeof(data)/sizeof(data[0]);

    radixSort(data,len);

    printf("排序后的数组: ");
    for (int i=0; i< len; i++)
    {
        printf("%d ",data[i]);
    }
    printf("\n");
    return SUCCESS;
}
