/*
Graph data structures and their operations.
*/
#ifndef GRAPH_H
#define GRAPH_H

#include <stddef.h>
#include "ds_type.h"

typedef struct {
    VertexType vex[MAXVEX];//每个顶点的数据
    EdgeType arc[MAXVEX][MAXVEX];//邻接矩阵，表示顶点之间的边
    size_t numVertexes, numEdges;//顶点数和边数
} Mat_Graph;//通过矩阵表示图

typedef struct enode {
    int edge_vex;//邻接表中的边对应的顶点索引
    struct enode* next;//指向下一条边的指针
} EdgeNode;

typedef struct vnode {
    int in; //入度值
    EdgeNode* head; //邻接表头指针
    VertexType data; //顶点名字
}VertexNode;

typedef VertexNode Adj_List[MAXVEX]; //Adj_List is an array of VertexNode, 
                                     //representing the adjacency list of the graph.

typedef struct {
    Adj_List adj_list;
    size_t numVertexes, numEdges;
} Adj_Graph;//邻接表表示的图. 每个顶点都在图中，每个顶点的邻接表存储了与该顶点相连的所有边。

typedef Adj_Graph* Adj_List_GraphPtr; //Adj_List_GraphPtr is a pointer to an Adj_Graph structure, 
                                      //representing a graph using an adjacency list.

#endif // GRAPH_H