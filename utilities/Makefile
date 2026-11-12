CC = gcc

# 头文件路径：src 的 .c 和 test 的 .c 都要能找到 include/stack.h
CFLAGS = -std=c99 -pedantic-errors -Wall -Wextra -Werror -g -O0 -Iinclude
CFLAGS += -MMD -MP

BIN_DIR  = bin
OBJ_DIR  = obj
TEST_DIR = test

# 库源文件（不含 main）
LIB_SRCS = $(wildcard src/*.c)
LIB_OBJS = $(patsubst src/%.c, $(OBJ_DIR)/%.o, $(LIB_SRCS))

# 测试源文件（含 main）
TEST_SRCS = $(wildcard $(TEST_DIR)/*.c)
TEST_OBJS = $(patsubst $(TEST_DIR)/%.c, $(OBJ_DIR)/%.o, $(TEST_SRCS))

# 每个测试源生成一个同名可执行文件
TEST_TARGETS = $(patsubst $(TEST_DIR)/%.c, $(BIN_DIR)/%, $(TEST_SRCS))

# 保留中间 .o 文件
.SECONDARY: $(LIB_OBJS) $(TEST_OBJS)

all: $(TEST_TARGETS)

# 链接：每个测试目标 = 自己的 .o + 库的 .o
$(BIN_DIR)/%: $(OBJ_DIR)/%.o $(LIB_OBJS)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) -o $@ $^

# 编译 src/*.c -> obj/*.o
$(OBJ_DIR)/%.o: src/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# 编译 test/*.c -> obj/*.o
$(OBJ_DIR)/%.o: $(TEST_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(BIN_DIR) $(OBJ_DIR)

-include $(LIB_OBJS:.o=.d) $(TEST_OBJS:.o=.d)

.PHONY: all clean