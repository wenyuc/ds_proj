CC = gcc
# 关键点：添加 -Iinclude 指定头文件搜索路径
CFLAGS = -std=c99 -pedantic-errors -Wall -Wextra -Werror -g -O0 -Iinclude
#自动依赖追踪
CFLAGS += -MMD -MP

BIN_DIR = bin
OBJ_DIR = obj
TARGET = $(BIN_DIR)/test_seqlist

# 自动找出 src/ 下所有的 .c 文件
SRCS = $(wildcard src/*.c)
# 将 src/xxx.c 替换为 obj/xxx.o
OBJS = $(patsubst src/%.c, $(OBJ_DIR)/%.o, $(SRCS))

all: $(TARGET)

$(TARGET): $(OBJS)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) -o $@ $^

# 编译规则：把 src/ 下的 .c 编译成 obj/ 下的 .o
# $< 代表 src/xxx.c，$@ 代表 obj/xxx.o
$(OBJ_DIR)/%.o: src/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(BIN_DIR) $(OBJ_DIR)

-include $(OBJS:.o=.d)

.PHONY: all clean