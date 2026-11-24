#include "graph.h"

int main(void)
{
    Mat_Graph matrix;
    Adj_Graph *adjacency = NULL;
    size_t edge_count = 0;
    size_t indegree_sum = 0;

    Status status = create_MatGraph(&matrix);
    CHECK(status == SUCCESS, "create matrix graph");
    if (status != SUCCESS) return 1;
    CHECK(matrix.numVertexes == 14, "matrix.numVertexes == 14");
    CHECK(matrix.numEdges == 20, "matrix.numEdges == 20");
    for (size_t i = 0; i < matrix.numVertexes; ++i) {
        CHECK(matrix.vex[i] == (VertexType)i, "matrix.vex[i] == (VertexType)i");
        for (size_t j = 0; j < matrix.numVertexes; ++j) {
            if (matrix.arc[i][j] != 0) {
                ++edge_count;
            }
        }
    }
    CHECK(edge_count == matrix.numEdges, "edge_count == matrix.numEdges");
    CHECK(matrix.arc[0][4] == 1, "matrix.arc[0][4] == 1");
    CHECK(matrix.arc[13][0] == 0, "matrix.arc[13][0] == 0");
    status = create_AdjGraph(matrix, &adjacency);
    CHECK(status == SUCCESS, "create adjacency graph");
    if (status != SUCCESS) {
        CHECK(destroy_MatGraph(&matrix) == SUCCESS, "destroy matrix graph");
        return 1;
    }
    CHECK(adjacency != NULL, "adjacency != NULL");
    if (adjacency == NULL) {
        CHECK(destroy_MatGraph(&matrix) == SUCCESS, "destroy matrix graph");
        return 1;
    }
    CHECK(adjacency->numVertexes == matrix.numVertexes, "adjacency->numVertexes == matrix.numVertexes");
    CHECK(adjacency->numEdges == matrix.numEdges, "adjacency->numEdges == matrix.numEdges");
    for (size_t i = 0; i < adjacency->numVertexes; ++i) {
        size_t outdegree = 0;
        size_t expected_outdegree = 0;
        size_t indegree = 0;
        EdgeNode *edge = adjacency->adj_list[i].head;
        CHECK(adjacency->adj_list[i].data == matrix.vex[i], "adjacency->adj_list[i].data == matrix.vex[i]");
        while (edge != NULL) {
            CHECK(edge->edge_vex >= 0, "edge->edge_vex >= 0");
            int valid_vertex = edge->edge_vex >= 0 &&
                               (size_t)edge->edge_vex < matrix.numVertexes;
            CHECK(valid_vertex, "edge points to a valid vertex");
            if (valid_vertex) {
                CHECK(matrix.arc[i][edge->edge_vex] == 1, "edge exists in matrix graph");
            }
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
        CHECK(outdegree == expected_outdegree, "outdegree == expected_outdegree");
        CHECK((size_t)adjacency->adj_list[i].in == indegree, "(size_t)adjacency->adj_list[i].in == indegree");
        indegree_sum += indegree;
    }
    CHECK(indegree_sum == matrix.numEdges, "indegree_sum == matrix.numEdges");
    CHECK(destroy_AdjGraph(adjacency) == SUCCESS, "destroy_AdjGraph(adjacency) == SUCCESS");
    CHECK(destroy_MatGraph(&matrix) == SUCCESS, "destroy_MatGraph(&matrix) == SUCCESS");
    return g_fail == 0 ? SUCCESS : ERROR;
}
