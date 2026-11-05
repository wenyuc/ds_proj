/*
 * 测试图的基本操作，包括创建图、Floyd算法求最短路径。
*/
#include <stdio.h>

#define MAXVEX 100
#define MAX 0x10000
#define MAXEDGE MAXVEX * MAXVEX
#define SUCCESS 0
#define ERROR 1

typedef int Status;
typedef int VertexType;
typedef int EdgeType;

typedef struct {
    VertexType vex[MAXVEX];
    EdgeType arc[MAXVEX][MAXVEX];
    int numVertexes, numEdges;
} Mat_Graph;

Status creategraph(Mat_Graph *G) {
    G->numVertexes = 9;
    G->numEdges = 16;
    
    //初始化边
    for (int i=0; i<G->numVertexes; i++) {
        G->vex[i] = i;   //顶点为整形
        for (int j=0; j<G->numVertexes; j++) {
            if (i==j) {
                G->arc[i][j]=0;
            }else {
                G->arc[i][j]=MAX;
            }
        }
    }

    //设置边的权重
    G->arc[0][1]=1;
    G->arc[0][2]=5;

    G->arc[1][2]=3;
    G->arc[1][3]=7;
    G->arc[1][4]=5;

    G->arc[2][4]=1;
    G->arc[2][5]=7;

    G->arc[3][4]=2;
    G->arc[3][6]=3;

    G->arc[4][5]=3;
    G->arc[4][6]=6;
    G->arc[4][7]=9;

    G->arc[5][7]=5;

    G->arc[6][7]=2;
    G->arc[6][8]=7;

    G->arc[7][8]=4;

    for (int i=0; i<G->numVertexes; i++) {
        for (int j=0; j<G->numVertexes; j++) {
            G->arc[j][i] = G->arc[i][j];
        }
    }
    return SUCCESS;
}

Status Floyd(Mat_Graph G) {
    int path[MAXVEX][MAXVEX]; //存储从vi到vj的最短路径中vj的前一个顶点
    int dist[MAXVEX][MAXVEX]; //存储从vi到vj的最短距离

    //初始化dist和path数组
    for (int i = 0; i < G.numVertexes; i++) {
        for (int j = 0; j < G.numVertexes; j++) {
            dist[i][j] = G.arc[i][j];
            path[i][j] = j;
        }
    }

    //Floyd算法核心.k:中转点，i:起点，j:终点
    for (int k = 0; k < G.numVertexes; k++) {
        for (int i = 0; i < G.numVertexes; i++) {
            for (int j = 0; j < G.numVertexes; j++) {
                if (dist[i][k] + dist[k][j] < dist[i][j]) {
                    dist[i][j] = dist[i][k] + dist[k][j];
                    path[i][j] = path[i][k];
                }
            }
        }
    }

    //输出最短路径
    int k;
    for (int i = 0; i < G.numVertexes; i++) {
        for (int j = i+1; j < G.numVertexes; j++) {
            printf("V%d->V%d weight: %d\n", i, j, dist[i][j]);
            k = path[i][j];
            printf("path: v%d",i);
            while (k != j) {
                printf("->v%d", k);
                k = path[k][j];
            }
            printf("->v%d\n", j);
        }
    }
    return SUCCESS;
}

int main(void) {
    int ret;
    Mat_Graph G;
    ret=creategraph(&G);
    if (ret == SUCCESS) {
        ret=Floyd(G);
        if (ret != SUCCESS) {return ERROR;}
    } else {return ERROR;}

    return SUCCESS;
}