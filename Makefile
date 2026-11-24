CC = gcc
AR = ar
CFLAGS = -std=c99 -pedantic-errors -Wall -Wextra -Werror -g -O0
CFLAGS += -MMD -MP
INCLUDES = -Iutilities/include

UTIL_SRCS = $(wildcard utilities/src/*.c)
UTIL_OBJS = $(patsubst utilities/src/%.c, utilities/obj/%.o, $(UTIL_SRCS))
UTIL_LIB = utilities/libutilities.a

APP_SRCS = $(wildcard app/src/*.c)
APP_OBJS = $(patsubst app/src/%.c, app/obj/%.o, $(APP_SRCS))
APP_TARGETS = $(patsubst app/src/%.c, app/bin/%, $(APP_SRCS))

all: $(UTIL_LIB) $(APP_TARGETS)

$(UTIL_LIB): $(UTIL_OBJS)
	$(AR) rcs $@ $^

utilities/obj/%.o: utilities/src/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

app/bin/%: app/obj/%.o $(UTIL_LIB)
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -o $@ $^

app/obj/%.o: app/src/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -rf app/bin app/obj utilities/obj $(UTIL_LIB)

-include $(UTIL_OBJS:.o=.d) $(APP_OBJS:.o=.d)

.PHONY: all clean
