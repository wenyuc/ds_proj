CC = gcc
CFLAGS = -std=c99 -pedantic-errors -Wall -Wextra -Werror -g -O0
LDLIBS = -lm -lpthread -lssl -lcrypto

$(shell mkdir -p bin)

SRCS = test_sort.c test_shellsort.c test_mergesort.c test_quicksort.c test_radixsort.c test_two_way_insert_sort.c
	   
TARGETS = $(addprefix bin/, $(basename $(SRCS)))

all: $(TARGETS)

bin/%: %.c
	$(CC) $(CFLAGS) -o $@ $< $(LDLIBS)
	@echo "Built $@"

clean:
	rm -f $(TARGETS)
	@echo "Cleaned $(TARGETS)"

.PHONY: all clean
