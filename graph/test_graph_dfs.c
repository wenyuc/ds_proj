/*
 * 测试图的基本操作，包括创建图、深度优先搜索（DFS）遍历图。
*/
#include <stdio.h>

#define MAXVEX 100
#define SUCCESS 0
#define ERROR 1

typedef int Status;
typedef char VertexType;
typedef int EdgeType;

typedef struct {
    VertexType vex[MAXVEX];
    EdgeType arc[MAXVEX][MAXVEX];
    int numVertexes, numEdges;
} Mat_Graph;

int visited[MAXVEX];

Status creategraph(Mat_Graph *G) {
    //创建图只包含顶点，没有边
    G->numVertexes = 9;
    G->numEdges = 15;
    G->vex[0]='A';
    G->vex[1]='B';
    G->vex[2]='C';
    G->vex[3]='D';
    G->vex[4]='E';
    G->vex[5]='F';
    G->vex[6]='G';
    G->vex[7]='H';
    G->vex[8]='I';

    for (int i=0; i<G->numVertexes; i++) {
        for (int j=0; j<G->numVertexes; j++) {
            G->arc[i][j] = 0;
        }
    }

    //A-B, A-F
    G->arc[0][1] = 1;
    G->arc[0][5] = 1;
    //B-C, B-G,B-I
    G->arc[1][2] = 1;
    G->arc[1][6] = 1;
    G->arc[1][8] = 1;
    //C-D, C-I
    G->arc[2][3] = 1;
    G->arc[2][8] = 1;
    //D-E, D-G, D-H, D-I
    G->arc[3][4] = 1;
    G->arc[3][6] = 1;
    G->arc[3][7] = 1;
    G->arc[3][8] = 1;
    //E-F, E-H
    G->arc[4][5] = 1;
    G->arc[4][7] = 1;
    //F-G
    G->arc[5][6] = 1;
    //G-H
    G->arc[6][7] = 1;

    for (int i=0; i<G->numVertexes; i++) {
        for (int j=0; j<G->numVertexes; j++) {
            G->arc[j][i] = G->arc[i][j];
        }
    }
    return SUCCESS;
}

Status dfs(Mat_Graph *G, int i) {
    visited[i] = 1;
    printf("%c ", G->vex[i]);
    for (int j = 0; j < G->numVertexes; j++) {
        if (G->arc[i][j] == 1 && !visited[j]) {  //i-j相连且j未访问过
            dfs(G, j);//递归访问与i相连的未访问过的结点j
        }
    }
    return SUCCESS;
}

int main(void) {
    int ret;
    Mat_Graph G;
    ret=creategraph(&G);
    if (ret == SUCCESS) {
        for (int i=0; i<G.numVertexes; i++) {
            visited[i] = 0;
        }
        dfs(&G, 0);
    } else {return ERROR;}

    return SUCCESS;
}