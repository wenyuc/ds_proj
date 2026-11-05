CC = gcc
CFLAGS = -std=c99 -pedantic-errors -Wall -Wextra -Werror -g -O0
LDLIBS = -lm -lpthread -lssl -lcrypto

$(shell mkdir -p bin)

SRCS = test_graph_dfs.c test_graph_bfs.c test_graph_prim.c test_graph_kruskal.c test_graph_dijkstra.c\
       test_graph_floyd.c test_topo_sort.c test_cp.c
	   
TARGETS = $(addprefix bin/, $(basename $(SRCS)))

all: $(TARGETS)

bin/%: %.c
	$(CC) $(CFLAGS) -o $@ $< $(LDLIBS)
	@echo "Built $@"

clean:
	rm -f $(TARGETS)
	@echo "Cleaned $(TARGETS)"

.PHONY: all clean
