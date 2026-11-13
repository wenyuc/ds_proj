CC = gcc
CFLAGS = -std=c99 -pedantic-errors -Wall -Wextra -Werror -g -O0
CFLAGS += -I../utilities/include
CFLAGS += -MMD -MP

BIN_DIR = bin
OBJ_DIR = obj
UTIL_OBJ_DIR = ../utilities/obj

APP_SRCS = $(wildcard src/*.c)
APP_OBJS = $(patsubst src/%.c, $(OBJ_DIR)/%.o, $(APP_SRCS))
TARGETS  = $(patsubst src/%.c, $(BIN_DIR)/%, $(APP_SRCS))

# app 使用顺序栈或链接栈实现
UTIL_LIBS = $(UTIL_OBJ_DIR)/stack_seq.o $(UTIL_OBJ_DIR)/stack_link.o

# 保留中间 .o 文件
.SECONDARY: $(APP_OBJS) $(UTIL_LIBS)

all: $(TARGETS)

$(BIN_DIR)/%: $(OBJ_DIR)/%.o $(UTIL_LIBS)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) -o $@ $^

$(OBJ_DIR)/%.o: src/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# 若 utilities 的*.o 不存在，先构建*.o
$(UTIL_LIBS)/%.o:
	$(MAKE) -C ../utilities obj/$*.o

clean:
	rm -rf $(BIN_DIR) $(OBJ_DIR)

-include $(APP_OBJS:.o=.d)
.PHONY: all clean
