#include <stdio.h>

int fibo_loop(int n)
{
    int last1=1;
    int last2=1;
    int result=0;

    for (int i=3; i<=n; i++)
    {
        result=last1+last2;
        last2=last1;
        last1=result;
    }
    return result;
}

int fibo(int n)
{
    if(n==1||n==2)
    {
        return 1;
    } else {
        return fibo(n-1)+fibo(n-2);
    }
}
int sum_n_loop(int n)
{
    int sum=0;
    for (int i=1; i<=n; i++)
    {
        sum+=i;
    }
    return sum;
}

int sum_n(int n)
{
    if (n==1)
    {
        return 1;
    } else {
        return sum_n(n-1)+n;
    }
}

int main(void)
{
    int n=10;
    printf("sum 1 to %d=%d\n",n,sum_n(n));

    n=8;
    printf("Fibonacci seq #%d=%d\n",n,fibo(n));

}