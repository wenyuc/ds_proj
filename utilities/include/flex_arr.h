/*
Header for Flex array

C11 6.7.2.1（结构和联合说明符）第 18 段：
作为特殊情况，具有多个命名成员的结构体的最后一个成员可以具有不完整数组类型；这样的成员称为柔性数组成员。
关键点：
必须是最后一个成员（last member）。
结构体必须至少有一个其他命名成员（不能只有柔性数组）。
柔性数组的元素类型必须完整（uint8_t 可以，void 不行）。
违反就是约束违反（constraint violation），编译器必须报错，不是警告。
*/

#include <stdint.h>

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

//frame_flex operations
struct frame_flex_c* create_flex_arr(frame_flex_c *f; const uint8_t len);
struct uart_frame* create_uart_frame(uart_frame *, const uint8_t len);
