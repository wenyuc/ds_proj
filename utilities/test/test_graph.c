/*
Unit Test Graph Operations
*/

#include <stdio.h>
#include "stack.h" //for array stack operations
#include "graph.h"

Status topological_sort(Adj_List_GraphPtr ALG) {
    EdgeNode *e;
    int curr;
    int k;

    //将所有入度为0的顶点入栈
    for (int i=0; i<(int)ALG->numVertexes; i++) {
        if (ALG->adj_list[i].in == 0) {
            array_push(i);
        }
    }
    while(!array_is_empty()) {
        array_pop(&curr);
        printf("V%d -> ",ALG->adj_list[curr].data);
        e = ALG->adj_list[curr].head;
        while(e != NULL) {
            k = e->edge_vex;
            ALG->adj_list[k].in--;
            if (ALG->adj_list[k].in == 0) {
                array_push(k);
            }
            e = e->next;
        }
    }
    printf("\n");
    return SUCCESS;
}

int main(void) {
    int ret;
    Mat_Graph G;  //邻接矩阵表示的图
    Adj_List_GraphPtr ALG=NULL;  //邻接表表示的图，ALG是指针，它指向Adj_Graph结构体.
    
    ret=create_MatGraph(&G);
    ret=create_AdjGraph(G, &ALG); //&ALG的类型为Adj_Graph**
    ret=topological_sort(ALG);
    destroy_AdjGraph(ALG);
    destroy_MatGraph(&G);
    
    if (ret == SUCCESS) return SUCCESS;
    return ERROR;
}
