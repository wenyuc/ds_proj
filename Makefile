CC = gcc
CFLAGS = -std=c99 -pedantic-errors -Wall -Wextra -Werror -g -O0
CFLAGS += -MMD -MP

# 头文件搜索路径（相对根目录）
INCLUDES = -Iutilities/include

BIN_DIR = bin
OBJ_DIR = obj

# ---- 库（utilities）----
LIB_SRCS = utilities/src/stack.c
LIB_OBJS = $(patsubst utilities/src/%.c, $(OBJ_DIR)/util_%.o, $(LIB_SRCS))

# ---- utilities 的测试 ----
UTIL_TEST_SRCS = utilities/test/test_stack.c
UTIL_TEST_OBJS = $(patsubst utilities/test/%.c, $(OBJ_DIR)/util_%.o, $(UTIL_TEST_SRCS))

# ---- app ----
APP_SRCS = app/src/test_expression.c
APP_OBJS = $(patsubst app/src/%.c, $(OBJ_DIR)/app_%.o, $(APP_SRCS))

# ---- 最终目标 ----
UTIL_TEST_BIN = $(BIN_DIR)/test_stack
APP_BIN       = $(BIN_DIR)/test_expression

all: $(UTIL_TEST_BIN) $(APP_BIN)

# 链接 utilities 测试：测试 .o + 库 .o
$(UTIL_TEST_BIN): $(UTIL_TEST_OBJS) $(LIB_OBJS)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) -o $@ $^

# 链接 app：app .o + 库 .o
$(APP_BIN): $(APP_OBJS) $(LIB_OBJS)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) -o $@ $^

# 编译 utilities/src/*.c -> obj/util_*.o
$(OBJ_DIR)/util_%.o: utilities/src/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# 编译 utilities/test/*.c -> obj/util_*.o
$(OBJ_DIR)/util_%.o: utilities/test/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# 编译 app/src/*.c -> obj/app_*.o
$(OBJ_DIR)/app_%.o: app/src/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# 保留中间 .o（避免 make 自动删除）
.SECONDARY: $(LIB_OBJS) $(UTIL_TEST_OBJS) $(APP_OBJS)

clean:
	rm -rf $(BIN_DIR) $(OBJ_DIR)

-include $(LIB_OBJS:.o=.d) $(UTIL_TEST_OBJS:.o=.d) $(APP_OBJS:.o=.d)

.PHONY: all clean