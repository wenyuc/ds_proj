/*
Graph Implementation
*/

#include <stdio.h>
#include <stdlib.h>
#include "graph.h"

//Create a matrix graph with a specific set of vertices and edges.
Status create_MatGraph(Mat_Graph *G) {
    G->numVertexes = 14;
    G->numEdges = 20;
    
    //初始化边
    for (int i=0; i<(int)G->numVertexes; i++) {
        G->vex[i] = i;   //顶点为整形
        for (int j=0; j<(int)G->numVertexes; j++) {
            G->arc[i][j] = 0;
        }
    }

    //设置边
    G->arc[0][4]=1;
    G->arc[0][5]=1;
    G->arc[0][11]=1;

    G->arc[1][2]=1;
    G->arc[1][4]=1;
    G->arc[1][8]=1;

    G->arc[2][5]=1;
    G->arc[2][6]=1;
    G->arc[2][9]=1;

    G->arc[3][2]=1;
    G->arc[3][13]=1;

    G->arc[4][7]=1;

    G->arc[5][8]=1;
    G->arc[5][12]=1;

    G->arc[6][5]=1;

    G->arc[8][7]=1;

    G->arc[9][10]=1;
    G->arc[9][11]=1;

    G->arc[10][13]=1;
    
    G->arc[12][9]=1;

    return SUCCESS;
}//create_MatGraph

//Create an adjacency list graph from a matrix graph.
Status create_AdjGraph(Mat_Graph G, Adj_Graph **ALG) {
    *ALG = (Adj_Graph*)malloc(sizeof(Adj_Graph));
    (*ALG)->numVertexes = (int)G.numVertexes;
    (*ALG)->numEdges = (int)G.numEdges;
    
    //创建邻接表表示的图
    for (int i=0; i<(int)G.numVertexes; i++) {
        (*ALG)->adj_list[i].data = G.vex[i];
        (*ALG)->adj_list[i].in = 0;
        (*ALG)->adj_list[i].head = NULL;
    }
    for (int i=0; i<(int)G.numVertexes; i++) {
        for (int j=0; j<(int)G.numVertexes; j++) {
            if (G.arc[i][j] == 1) { //i->j有连接
                EdgeNode* edge = (EdgeNode*)malloc(sizeof(EdgeNode));
                edge->edge_vex = j;
                edge->next = (*ALG)->adj_list[i].head; //头插法插入顶点i
                (*ALG)->adj_list[i].head = edge;
                (*ALG)->adj_list[j].in++; //顶点j增加入度
            }
        }
    }
    return SUCCESS;
}//create_AdjGraph

Status display_MatGraph(Mat_Graph* G) 
{
    for (int i=0; i<(int)G->numVertexes; i++) {
        for (int j=0; j<(int)G->numVertexes; j++) {
            printf("%d ", G->arc[i][j]);
        }
        printf("\n");
    }
    return SUCCESS;
}//display_MatGraph

Status display_AdjGraph(Adj_List_GraphPtr G) {
    for (int i=0; i<(int)G->numVertexes; i++) {
        printf("%d: ", G->adj_list[i].data);
        EdgeNode* edge = G->adj_list[i].head;
        while (edge != NULL) {
            printf("%d ", edge->edge_vex);
            edge = edge->next;
        }
        printf("\n");
    }
    return SUCCESS;
}

Status destroy_MatGraph(Mat_Graph* G) {
    (void)G;//避免编译器警告未使用参数
    return SUCCESS;//矩阵图没有动态分配的内存，直接返回成功
}

Status destroy_AdjGraph(Adj_List_GraphPtr G) {
    for (int i=0; i<(int)G->numVertexes; i++) {
        EdgeNode* edge = G->adj_list[i].head;
        while (edge != NULL) {
            EdgeNode* temp = edge;
            edge = edge->next;
            free(temp);
        }
    }
    free(G);
    return SUCCESS;
}
