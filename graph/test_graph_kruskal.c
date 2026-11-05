/*
 * 测试图的基本操作，包括创建图、用Kruskal算法创建最小生成树。
*/
#include <stdio.h>

#define MAXVEX 100
#define MAXEDGES (MAXVEX) * (MAXVEX)
#define MAX 0x7fffffff
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

typedef struct {
    int from;
    int to;
    int weight;
} Edge;

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
            if (i==j)
                G->arc[i][j] = 0;
            else
                G->arc[i][j] = MAX;
        }
    }

    //A-B, A-F
    G->arc[0][1] = 10;
    G->arc[0][5] = 11;
    //B-C, B-G,B-I
    G->arc[1][2] = 18;
    G->arc[1][6] = 16;
    G->arc[1][8] = 12;
    //C-D, C-I
    G->arc[2][3] = 22;
    G->arc[2][8] = 8;
    //D-E, D-G, D-H, D-I
    G->arc[3][4] = 20;
    G->arc[3][6] = 24;
    G->arc[3][7] = 16;
    G->arc[3][8] = 21;
    //E-F, E-H
    G->arc[4][5] = 26;
    G->arc[4][7] = 7;
    //F-G
    G->arc[5][6] = 17;
    //G-H
    G->arc[6][7] = 19;

    for (int i=0; i<G->numVertexes; i++) {
        for (int j=0; j<G->numVertexes; j++) {
            G->arc[j][i] = G->arc[i][j];
        }
    }
    return SUCCESS;
}


Status kruskal1(Mat_Graph *G) {
    int i, j;
    int parent[MAXVEX]; //用于存储每个顶点的父节点
    int rank[MAXVEX]; //用于存储每个顶点的秩
    Edge edges[MAXVEX * MAXVEX]; //存储所有边
    int edge_count = 0; //边的数量

    //初始化每个顶点的父节点和秩
    for (i = 0; i < G->numVertexes; i++) {
        parent[i] = i;
        rank[i] = 0;
    }

    //收集所有边
    for (i = 0; i < G->numVertexes; i++) {
        for (j = i + 1; j < G->numVertexes; j++) {
            if (G->arc[i][j] != MAX) {
                edges[edge_count].from = i;
                edges[edge_count].to = j;
                edges[edge_count].weight = G->arc[i][j];
                edge_count++;
            }
        }
    }

    //对边按权重排序（这里使用简单的冒泡排序）
    for (i = 0; i < edge_count - 1; i++) {
        for (j = 0; j < edge_count - i - 1; j++) {
            if (edges[j].weight > edges[j + 1].weight) {
                Edge temp = edges[j];
                edges[j] = edges[j + 1];
                edges[j + 1] = temp;
            }
        }
    }

    //构建最小生成树
    for (i = 0; i < edge_count; i++) {
        int from = edges[i].from;
        int to = edges[i].to;
        int weight = edges[i].weight;

        //查找每个顶点的根节点
        int root_from = from;
        while (parent[root_from] != root_from) {
            root_from = parent[root_from];
        }
        int root_to = to;
        while (parent[root_to] != root_to) {
            root_to = parent[root_to];
        }

        //如果两个顶点的根节点不同，则合并
        if (root_from != root_to) {
            if (rank[root_from] < rank[root_to]) {
                parent[root_from] = root_to;
            } else if (rank[root_from] > rank[root_to]) {
                parent[root_to] = root_from;
            } else {
                parent[root_to] = root_from;
                rank[root_from]++;
            }
            printf("%c-%c:%d\n", G->vex[from], G->vex[to], weight);
        }
    }

    return SUCCESS;
}

Status sortEdges(Edge edges[], int edge_count) {
    for (int i = 0; i < edge_count; i++) {
        for (int j = i+1; j < edge_count; j++) {
            if (edges[i].weight > edges[j].weight) {
                Edge temp = edges[i];
                edges[i] = edges[j];
                edges[j] = temp;
            }
        }
    }
    return SUCCESS;
}

int find(int *parent, int index) {
    while(parent[index]>0) {
        index = parent[index];
    }
    return index;
}

Status kruskal(Mat_Graph *G) {
    Edge edges[MAXEDGES]; //存储所有边
    int k=0;  //边的数量

    for (int i = 0; i < G->numVertexes; i++) {
        for (int j = i + 1; j < G->numVertexes; j++) {
            if (G->arc[i][j] != MAX) {
                edges[k].from = i;
                edges[k].to = j;
                edges[k].weight = G->arc[i][j];
                k++;
            }
        }
    }
    sortEdges(edges, k);
    
    int parent[MAXVEX];
    for(int i=0; i<G->numVertexes; i++) {
        parent[i] = 0;
    }

    int n,m;
    for (int i = 0; i < k; i++) {
        n=find(parent, edges[i].from);//4
        m=find(parent, edges[i].to);//7
        if(n != m) {
            parent[n] = m;//边n->m是最小权值的边
            printf("%c-%c:%d\n", G->vex[edges[i].from], G->vex[edges[i].to], edges[i].weight);
        }
    }

    return SUCCESS;
}
int main(void) {
    int ret;
    Mat_Graph G;
    ret=creategraph(&G);
    if (ret == SUCCESS) {
        kruskal(&G);
    } else {return ERROR;}

    return SUCCESS;
}