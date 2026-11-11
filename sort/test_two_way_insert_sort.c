#include <stdio.h>

#define SUCCESS 0
#define FAILURE 1
typedef int Status;

/*按教材图10.2的样式打印一步排序后辅助数组d的状态：
 *后半区为d[0..final]（含处于中间位置的记录d[0]），前半区为d[first..len-1]，
 *两个区内部都已非递减有序；first=0表示前半区还没有记录。
 *对照教材第268页图10.2：图中每行先画d[0]之后的有序表，再画d[0]之前的有序表。*/
static void printStep(int step, const int *d, int len, int first, int final)
{
    int k;

    printf("  i=%-2d d[0]之后: (", step);
    printf("%d", d[0]);
    for (k = 1; k <= final; k++)
    {
        printf(" %d", d[k]);
    }
    printf(")  d[0]之前: (");
    if (first == 0)
    {
        printf("空");
    }
    else
    {
        printf("%d", d[first]);
        for (k = first + 1; k < len; k++)
        {
            printf(" %d", d[k]);
        }
    }
    printf(")  first=%d, final=%d\n", first, final);
}//end of printStep

/*
 * 2-路插入排序（Two-way Insertion Sort）
 * 教材：严蔚敏《数据结构(C语言版)》第10章10.2.2节“其他插入排序”（第266~270页）。
 *
 * 教材做法（第267页，具体代码教材留作习题，本文件按其思想与图10.2实现）：
 *  1.另设一个和待排序记录同类型的辅助数组d（本代码中d与data等长）；
 *  2.将第1个记录放入d的首位置，并把它看成排好序序列中处于中间位置的
 *    记录（教材为1下标，记作d[1]=L.r[1]；本代码为0下标，记作d[0]=data[0]）；
 *  3.从第2个记录起依次插入：先将待插关键字与中间记录d[0]比较，若小于
 *    d[0]，则插入到d[0]之前的有序表中；反之插入到d[0]之后的有序表中；
 *  4.把d看成循环向量，设first和final分别指示排序过程中得到的有序序列
 *    中第1个和最后1个记录在d中的位置；
 *  5.排序结束后，把d中从first到final（循环）的有序序列复制回data。
 *
 * 教材结论：2-路插入排序将移动记录的次数减少到约n^2/8，但为此需要n个
 * 记录的辅助空间；当第1个记录是待排序记录中关键字最小或最大的记录时，
 * 2-路插入排序完全失去它的优越性（见main中的测试2、测试3）。
 *
 * verbose不为0时，每插入一个记录就按图10.2的样式打印一次d的状态。
 */
Status twoWayInsertSort(int *data, int len, int verbose)
{
    if (data == NULL || len <= 0)
    {
        return FAILURE;
    }

    int d[len];    /*辅助数组：教材中的循环向量d*/
    int first;     /*有序序列中第1个（最小）记录在d中的位置*/
    int final;     /*有序序列中最后1个（最大）记录在d中的位置*/
    int i, j;

    /*第1个记录放入d[0]（教材：d[1]=L.r[1]），作为有序序列的中间记录*/
    d[0] = data[0];
    first = final = 0;
    if (verbose != 0)
    {
        printStep(1, d, len, first, final);
    }

    /*从第2个记录起依次插入*/
    for (i = 1; i < len; i++)
    {
        if (data[i] < d[0])
        {
            /*情况1：待插关键字小于中间记录d[0]，
              插入到d[0]之前的有序表（前半区，向first方向生长）*/
            if (data[i] < d[first])
            {
                /*小于当前最小记录：first在循环向量中后退1格直接放入，
                  不需要移动其它记录*/
                first = (first - 1 + len) % len;
                d[first] = data[i];
            }
            else
            {
                /*插入在前半区中间：first先后退1格腾出空位，再把前半区
                  中关键字不大于data[i]的记录逐个向first方向平移1格，
                  最后把待插记录放入空出的位置*/
                first = (first - 1 + len) % len;
                for (j = first + 1; j < len && d[j] <= data[i]; j++)
                {
                    d[j - 1] = d[j];
                }
                d[j - 1] = data[i];
            }
        }
        else
        {
            /*情况2：待插关键字不小于中间记录d[0]，
              插入到d[0]之后的有序表（后半区，向final方向生长）*/
            if (data[i] > d[final])
            {
                /*大于当前最大记录：final在循环向量中前进1格直接放入，
                  不需要移动其它记录*/
                final = (final + 1) % len;
                d[final] = data[i];
            }
            else
            {
                /*插入在后半区中间：把后半区中关键字大于data[i]的记录
                  逐个向final方向平移1格（移动方式与直接插入排序相同），
                  再把待插记录放入空出的位置*/
                for (j = final; data[i] < d[j]; j--)
                {
                    d[j + 1] = d[j];
                }
                d[j + 1] = data[i];
                final = (final + 1) % len;
            }
        }

        if (verbose != 0)
        {
            printStep(i + 1, d, len, first, final);
        }
    }

    /*排序结束：从first开始把d中循环存放的有序序列复制回data*/
    for (i = 0, j = first; i < len; i++, j = (j + 1) % len)
    {
        data[i] = d[j];
    }

    return SUCCESS;
}//end of twoWayInsertSort

/*打印一行数组*/
static void printArray(const char *label, const int *data, int len)
{
    printf("%s", label);
    for (int i = 0; i < len; i++)
    {
        printf(" %d", data[i]);
    }
    printf("\n");
}//end of printArray

/*直接插入排序：结果作为参照，用于验证2-路插入排序的正确性*/
static void insertionSort(int *data, int len)
{
    for (int i = 1; i < len; i++)
    {
        int key = data[i];
        int j = i - 1;
        while (j >= 0 && data[j] > key)
        {
            data[j + 1] = data[j];
            j--;
        }
        data[j + 1] = key;
    }
}//end of insertionSort

/*执行一组测试：2-路插入排序后与直接插入排序的结果逐一比对*/
static Status runCase(const char *name, const int *origin, int len, int verbose)
{
    int data[len];  /*2-路插入排序的输入*/
    int refer[len]; /*直接插入排序的输入*/
    int same = 1;

    for (int i = 0; i < len; i++)
    {
        data[i] = origin[i];
        refer[i] = origin[i];
    }

    printf("== %s ==\n", name);
    printArray("排序前:", data, len);
    if (verbose != 0)
    {
        printf("排序过程中d的状态（i为记录序号，可与教材图10.2对照）：\n");
    }

    if (twoWayInsertSort(data, len, verbose) != SUCCESS)
    {
        printf("排序失败！\n\n");
        return FAILURE;
    }

    insertionSort(refer, len);
    printArray("排序后:", data, len);

    for (int i = 0; i < len; i++)
    {
        if (data[i] != refer[i])
        {
            same = 0;
            break;
        }
    }
    printf("验证（与直接插入排序结果一致）: %s\n\n", same == 1 ? "通过" : "失败");
    return same == 1 ? SUCCESS : FAILURE;
}//end of runCase

int main(void)
{
    int failed = 0;

    /*测试1：教材图10.2使用的关键字序列（式(10-4)），逐步打印d的状态，
      输出可与教材第268页图10.2逐行对照*/
    int book[] = {49, 38, 65, 97, 76, 13, 27, 49};
    int bookLen = (int)(sizeof(book) / sizeof(book[0]));
    if (runCase("测试1 教材图10.2示例", book, bookLen, 1) != SUCCESS)
    {
        failed = 1;
    }

    /*测试2：第1个记录是最大关键字。教材指出此时2-路插入排序完全失去
      优越性（记录都插到d[0]之前的有序表），但排序结果仍应正确*/
    int maxFirst[] = {90, 30, 20, 10, 50, 40, 80, 60, 70};
    int maxFirstLen = (int)(sizeof(maxFirst) / sizeof(maxFirst[0]));
    if (runCase("测试2 第1个记录是最大关键字", maxFirst, maxFirstLen, 0) != SUCCESS)
    {
        failed = 1;
    }

    /*测试3：第1个记录是最小关键字，同样失去优越性，结果仍应正确*/
    int minFirst[] = {10, 30, 20, 90, 50, 40, 80, 60, 70};
    int minFirstLen = (int)(sizeof(minFirst) / sizeof(minFirst[0]));
    if (runCase("测试3 第1个记录是最小关键字", minFirst, minFirstLen, 0) != SUCCESS)
    {
        failed = 1;
    }

    /*测试4：一般数据，含重复关键字*/
    int dup[] = {5, 8, 2, 9, 5, 1, 7, 3, 8, 0, 6, 4};
    int dupLen = (int)(sizeof(dup) / sizeof(dup[0]));
    if (runCase("测试4 含重复关键字的一般数据", dup, dupLen, 0) != SUCCESS)
    {
        failed = 1;
    }

    /*测试5：只有1个记录*/
    int one[] = {42};
    int oneLen = (int)(sizeof(one) / sizeof(one[0]));
    if (runCase("测试5 只有1个记录", one, oneLen, 0) != SUCCESS)
    {
        failed = 1;
    }

    if (failed != 0)
    {
        printf("存在失败的测试！\n");
        return FAILURE;
    }

    printf("全部测试通过。\n");
    return SUCCESS;
}//end of main
