#include <assert.h>
#include "graph.h"

int main(void)
{
    Mat_Graph matrix;
    Adj_Graph *adjacency = NULL;
    size_t edge_count = 0;
    size_t indegree_sum = 0;

    assert(create_MatGraph(&matrix) == SUCCESS);
    assert(matrix.numVertexes == 14);
    assert(matrix.numEdges == 20);
    for (size_t i = 0; i < matrix.numVertexes; ++i) {
        assert(matrix.vex[i] == (VertexType)i);
        for (size_t j = 0; j < matrix.numVertexes; ++j) {
            if (matrix.arc[i][j] != 0) {
                ++edge_count;
            }
        }
    }
    assert(edge_count == matrix.numEdges);
    assert(matrix.arc[0][4] == 1);
    assert(matrix.arc[13][0] == 0);
    assert(create_AdjGraph(matrix, &adjacency) == SUCCESS);
    assert(adjacency != NULL);
    assert(adjacency->numVertexes == matrix.numVertexes);
    assert(adjacency->numEdges == matrix.numEdges);
    for (size_t i = 0; i < adjacency->numVertexes; ++i) {
        size_t outdegree = 0;
        size_t expected_outdegree = 0;
        size_t indegree = 0;
        EdgeNode *edge = adjacency->adj_list[i].head;
        assert(adjacency->adj_list[i].data == matrix.vex[i]);
        while (edge != NULL) {
            assert(edge->edge_vex >= 0);
            assert((size_t)edge->edge_vex < matrix.numVertexes);
            assert(matrix.arc[i][edge->edge_vex] == 1);
            ++outdegree;
            edge = edge->next;
        }
        for (size_t j = 0; j < adjacency->numVertexes; ++j) {
            if (matrix.arc[i][j] == 1) {
                ++expected_outdegree;
            }
            if (matrix.arc[j][i] == 1) {
                ++indegree;
            }
        }
        assert(outdegree == expected_outdegree);
        assert((size_t)adjacency->adj_list[i].in == indegree);
        indegree_sum += indegree;
    }
    assert(indegree_sum == matrix.numEdges);
    assert(destroy_AdjGraph(adjacency) == SUCCESS);
    assert(destroy_MatGraph(&matrix) == SUCCESS);
    return SUCCESS;
}
