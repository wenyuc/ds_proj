/*
 * 测试图的基本操作，包括创建图、Dijkstra算法求最短路径。
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

//返回下一次要观察哪个结点
int choose(int dist[], int visited[], int numVertexes) {
    int min = MAX;
    int minPos = -1;
    for (int j = 0; j < numVertexes; j++) {
        if (!visited[j] && dist[j] <= min) {
            min = dist[j];
            minPos = j;
        }
    }
    return minPos;
}

Status dijkstra(Mat_Graph G, int start){
    int visited[MAXVEX]; //标记顶点是否已访问
    int path[MAXVEX]; //存储最短路径的前驱节点, path[n]的值表示从start到n的最短路径中n的前一个顶点
    int dist[MAXVEX]; //存储从start到每个顶点(数组下标)的最短距离

    //初始化dist、visited和path数组
    for (int i = 0; i < G.numVertexes; i++) {
        dist[i] = G.arc[start][i];
        visited[i] = 0;
        path[i] = -1;
    }
    dist[start] = 0;
    visited[start] = 1;

    int next; //下一个要观察的顶点
    for(int i=1; i<G.numVertexes; i++) {
        next = choose(dist, visited, G.numVertexes);
        if (next == -1) break;
        //printf("%d ",next);
        visited[next] = 1;//标记为已访问
        //得到从start到next的最短路径后，更新其他顶点的距离，并更新path数组
        for (int j = 0; j < G.numVertexes; j++) {
                if ( !visited[j] && dist[next] + G.arc[next][j] < dist[j]) {
                    dist[j] = dist[next] + G.arc[next][j]; //更新从start到j的最短距离
                    path[j] = next;//离start到j的最短路径中j的前一个顶点是next
                }
            
        }
    }

    //打印从start到每个顶点的最短距离和最短路径
    //或者压栈
    for (int i = start + 1; i < G.numVertexes; i++) {
        printf("从V%d到V%d的最短距离: %d\n", start, i, dist[i]);
        printf("最短路径: ");
        printf("V%d<- ", i);
        int j = i;
        while (path[j] != -1) {
            printf("V%d<- ", path[j]);
            j = path[j];
        }
        printf("V%d\n", start);
    }
    return SUCCESS;
}

void dijkstra1(Mat_Graph *G, int start) {
    int dist[MAXVEX]; //存储从start到每个顶点的最短距离
    int visited[MAXVEX]; //标记顶点是否已访问

    for (int i = 0; i < G->numVertexes; i++) {
        dist[i] = MAX;
        visited[i] = 0;
    }
    dist[start] = 0;

    for (int i = 0; i < G->numVertexes; i++) {
        int min = MAX;
        int u = -1;
        for (int j = 0; j < G->numVertexes; j++) {
            if (!visited[j] && dist[j] <= min) {
                min = dist[j];
                u = j;
            }
        }
        if (u == -1) break;
        visited[u] = 1;
        for (int v = 0; v < G->numVertexes; v++) {
            if (!visited[v] && G->arc[u][v] != 0 && dist[u] + G->arc[u][v] < dist[v]) {
                dist[v] = dist[u] + G->arc[u][v];
            }
        }
    }

    for (int i = 0; i < G->numVertexes; i++) {
        printf("%c: %d\n", G->vex[i], dist[i]);
    }
}

int main(void) {
    int ret;
    Mat_Graph G;
    ret=creategraph(&G);
    if (ret == SUCCESS) {
        int start=0;
        dijkstra(G, start);
        //dijkstra1(&G, start);
    } else {return ERROR;}

    return SUCCESS;
}