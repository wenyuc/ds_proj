/* 字符串匹配KMP算法
- 基于模式串确定next数组
- 利用next数组完成字符串匹配
- 在匹配过程中，发生字符不匹配情况时，next数组用来帮助确定下一次的匹配位置.
如何从模式串生成next数组？
遍历模式串，获取每个字符前面的内容。根据该内容的前、后缀相同的最大长度确定next数组的值。
如模式串“abaabc"
第一个字符'a'没有前面的内容，所以next[0] = -1;
第二个字符'b'前面的内容是'a'，前缀和后缀相同的最大长度是0，所以next[1] = 0;
第三个字符'a'前面的内容是'ab'，前缀为'a',后缀为'b',前缀和后缀相同的最大长度是0，所以next[2] = 0;
第四个字符'a'前面的内容是'aba'，前缀为'a'或'ab',后缀为'a'或'ba',前缀和后缀相同的最大长度是1，所以next[3] = 1;
第五个字符'b'前面的内容是'abaa'，前缀为'a'或'ab'或'aba',后缀为'a'或'aa'或'baa',前缀和后缀相同的最大长度是1，所以next[4] = 1;
第六个字符'c'前面的内容是'abaab'，前缀为'a'或'ab'或'aba'或'abaa',后缀为'b'或'ab'或'aab'或'baab',前缀和后缀相同的最大长度是2，所以next[5] = 2;
前缀和后缀相同的最大长度: 0, 0, 1, 1, 2
next数组: -1, 0, 0, 1, 1, 2
*/

#include <stdio.h>
#include <string.h>

#define MAXSIZE 100

void kmp(char* str1, char* pattern)
{
    int next[MAXSIZE];
    int i = 0, j = -1;
    int len = strlen(pattern);
    next[0] = -1;

    //根据pattern生成next数组
    while (i < len)
    {
        if (j == -1 || pattern[i] == pattern[j])
        {
            i++;
            j++;
            next[i] = j;
        }
        else
        {
            j = next[j];
        }
    }

    //根据next数组在str中匹配pattern
    i = 0;
    j = 0;
    int str_len = strlen(str1);
    while (i < str_len && j < len)
    {
        if (j == -1 || str1[i] == pattern[j])
        {
            i++;
            j++;
        }
        else
        {
            j = next[j];
        }
    }
    if (j == len)
    {
        printf("Pattern found at index %d\n", i - j);
    }
    else
    {
        printf("Pattern not found\n");
    }
}//end of kmp

int main(void) 
{
    char* str1="abaabaabacacaabaabcc";
    char* pattern="abaabc";
    kmp(str1, pattern);

    return 0;
}