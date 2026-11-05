#include <stdio.h>

typedef enum
{
    mon=1, tue, wed, thu, fri, sat, sun
}weekday;

typedef enum 
{
    false, true
}bool;

int main(void)
{
    weekday a;
    a=mon;
    weekday b;
    b=tue;
    bool f;
    f=false;
    bool t;
    t=true;
    
    printf("a=%d\n",a);
    printf("b=%d\n",b);
    printf("f=%d\n",f);
    printf("t=%d\n",t);
    return 0;

}