/*
flex array 柔性数组
C11 6.7.2.1（结构和联合说明符）第 18 段：
作为特殊情况，具有多个命名成员的结构体的最后一个成员可以具有不完整数组类型；这样的成员称为柔性数组成员。
关键点：
必须是最后一个成员（last member）。
结构体必须至少有一个其他命名成员（不能只有柔性数组）。
柔性数组的元素类型必须完整（uint8_t 可以，void 不行）。
违反就是约束违反（constraint violation），编译器必须报错，不是警告。
*/

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

struct frame_ptr{
    uint8_t len;
    int *data;
};

struct frame_flex{
    uint8_t len;// 偏移 0，占 1 字节
    int data[];//偏移4（因int需4字节对齐，中间要补3字节padding)
};

//字符流的柔性数组
struct frame_flex_c{
    uint8_t len;
    uint8_t data[];
};

//嵌入式里最合适的应用是定长头加变长负载。
struct uart_frame {
    uint8_t sof;
    uint8_t cmd;
    uint16_t len;
    uint8_t payload[];
};

int main(void)
{
    int n=3;

    //指针版的结构需要两次malloc，两次释放.
    //头部(len)和数据在内存中的地址可能不连续。访问是 f->data[i]，
    printf("===frame ptr===\n");
    printf("size of struct frame_ptr: %zu\n", sizeof(struct frame_ptr));//16bytes
    struct frame_ptr *f=(struct frame_ptr*)malloc(sizeof(*f));
    assert(f != NULL);
    f->len=(uint8_t)n;
    f->data=(int*)malloc(n);
    assert(f->data != NULL);
    f->data[0]=77;f->data[1]=78;f->data[2]=79;
    
    printf("f address:%p f len:%d address:%p f data address:%p\n",(void*)f, f->len, (void*)&f->len,(void*)f->data);
    printf("data: %d %d %d\n", f->data[0],f->data[1],f->data[2]);
    printf("data address: %p %p %p\n",(void*)&(f->data[0]),(void*)&(f->data[1]),(void*)&(f->data[2]));

    printf("data: %d %d %d\n", *(f->data),*(f->data+1),*(f->data+2));
    printf("data address: %p %p %p\n",(void*)(f->data), (void*)(f->data+1),(void*)(f->data+2));

    free(f->data);
    free(f);
    f=NULL;

    //数组版的只需一次malloc
    //头部和数据连成一块。分配一次，释放一次，地址连续。访问还是 f->data[i]，
    printf("===frame flex===\n");
    printf("size of struct frame_flex: %zu\n", sizeof(struct frame_flex)); //4字节，

    struct frame_flex *ff=(struct frame_flex *)malloc(sizeof(*ff)+n * sizeof(ff->data[0]));
    assert(ff != NULL);
    ff->len=(uint8_t)n;
    ff->data[0]=0xA5;ff->data[1]=0xAA;ff->data[2]=0xA0;
    
    //类型为uint8_t的变量为1字节，但在结构体内有3字节的paddig
    //语义上“柔性数组紧跟 len”的直觉不成立，且浪费 3 字节/帧。若在意内存，可加 #pragma pack(1) 或用 char data[]
    printf("ff address:%p ff len:%d address:%p ff data address:%p\n",(void*)ff, ff->len, (void*)&ff->len,(void*)ff->data);
    printf("data: %d %d %d\n", ff->data[0],ff->data[1],ff->data[2]);
    printf("data address: %p %p %p\n",(void*)&(ff->data[0]),(void*)&(ff->data[1]),(void*)&(ff->data[2]));

    printf("data: %d %d %d\n", *(ff->data),*(ff->data+1),*(ff->data+2));
    printf("data address: %p %p %p\n",(void*)(ff->data), (void*)(ff->data+1),(void*)(ff->data+2));

    free(ff);
    ff=NULL;

    //字符版柔性数组
    printf("===frame flex char===\n");
    printf("size of struct frame_flex_c: %zu\n", sizeof(struct frame_flex_c)); //1字节，   
    
    struct frame_flex_c *fc=(struct frame_flex_c *)malloc(sizeof(*fc)+n * sizeof(fc->data[0]));
    assert(fc != NULL);
    fc->len=(uint8_t)n;
    fc->data[0]=0xA5;fc->data[1]=0xAA;fc->data[2]=0xA0;
    
    //类型为uint8_t的变量为1字节，现在就没有padding
    printf("fc address:%p fc len:%d address:%p fc data address:%p\n",(void*)fc, fc->len, (void*)&fc->len,(void*)fc->data);
    printf("data: %d %d %d\n", fc->data[0],fc->data[1],fc->data[2]);
    printf("data address: %p %p %p\n",(void*)&(fc->data[0]),(void*)&(fc->data[1]),(void*)&(fc->data[2]));
    free(fc);

    //uart_frame
    printf("===uart frame===\n");
    printf("size of struct uart_frame: %zu\n", sizeof(struct uart_frame)); //4字节，   
    uint8_t payload_len=4;
    struct uart_frame *uf=(struct uart_frame*)malloc(sizeof(struct uart_frame)+ n*sizeof(uf->payload[0]));

    uf->sof=0xA0;
    uf->cmd=0xA5;
    uf->len=payload_len;
    uf->payload[0]='S';uf->payload[1]='T';uf->payload[2]='P';uf->payload[3]='\0';

    printf("uf address:%p uf payload address:%p\n",(void*)uf, (void*)uf->payload);
    printf("data: %c %c %c %c\n", uf->payload[0],uf->payload[1],uf->payload[2],uf->payload[3]);
    printf("data address: %p %p %p %p\n",(void*)&(uf->payload[0]),(void*)&(uf->payload[1]),(void*)&(uf->payload[2]),(void*)&(uf->payload[3]));
    free(uf);

    return 0;
}