CC = gcc
CFLAGS = -std=c99 -pedantic-errors -Wall -Wextra -Werror -g -O0
LDLIBS = -lm -lpthread -lssl -lcrypto

$(shell mkdir -p bin)

SRCS = search.c bst_search.c bst_insert.c bst_delete.c
	   
TARGETS = $(addprefix bin/, $(basename $(SRCS)))

all: $(TARGETS)

bin/%: %.c
	$(CC) $(CFLAGS) -o $@ $< $(LDLIBS)
	@echo "Built $@"

clean:
	rm -f $(TARGETS)
	@echo "Cleaned $(TARGETS)"

.PHONY: all clean
