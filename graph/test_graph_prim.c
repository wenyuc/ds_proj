/*
 * 测试图的基本操作，包括创建图、用Prim算法创建最小生成树。
*/
#include <stdio.h>

#define MAXVEX 100
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

Status prim_1(Mat_Graph *G) {
    int i,j,k;
    int min;
    int lowcost[MAXVEX];
    int closest[MAXVEX];

    for (i=0; i<G->numVertexes; i++) {
        lowcost[i] = MAX;
        closest[i] = -1;
    }

    lowcost[0] = 0;
    closest[0] = 0;

    for (i=1; i<G->numVertexes; i++) {
        min = MAX;
        k = -1;
        for (j=0; j<G->numVertexes; j++) {
            if (lowcost[j] != 0 && lowcost[j] < min) {
                min = lowcost[j];
                k = j;
            }
        }
        if (k == -1) break;
        printf("%c-%c:%d\n", G->vex[closest[k]], G->vex[k], G->arc[closest[k]][k]);
        lowcost[k] = 0;
        for (j=0; j<G->numVertexes; j++) {
            if (G->arc[k][j] < lowcost[j]) {
                lowcost[j] = G->arc[k][j];
                closest[j] = k;
            }
        }
    }
    return SUCCESS;
}

Status prim(Mat_Graph *G) {
    int i,j,k;
    int min;
    int weight[MAXVEX]; //候选边的权值
    int vex_index[MAXVEX]; //候选边的顶点索引,值表示出发点，下标表示终点

    //先从顶点A开始
    weight[0]=0;
    vex_index[0]=0;
    for (i=1; i<G->numVertexes; i++) {
        weight[i]=G->arc[0][i];
        vex_index[i]=0;
    }

    for (int i=1;i<G->numVertexes;i++) {
        min = MAX;
        j=0;
        k=0;
        //找到最小的权值，与之连接
        while(j<G->numVertexes) {
            if (weight[j] != 0 && weight[j] < min) {
                min = weight[j];  //从候选权值中找到最小的权值weight[j]
                k = j;  //k表示权值最小的顶点的下标
            }
            j++;
        }
        //printf("(%d %d)\n", vex_index[k], k); //表示vex_index[k]与k之间的边是权值最小的边
        printf("(%c %c)\n", G->vex[vex_index[k]], G->vex[k]); //将顶点转换成可读字符
        weight[k] = 0; //将当前选中的顶点标记为已加入最小生成树，避免重复选择

        //寻找新连接结点的待选路径
        for (j=0; j<G->numVertexes; j++) {
            //与新结点的可连接的边进行比较，有比weight小的就替换
            if (weight[j] != 0 && G->arc[k][j] < weight[j]) {
                weight[j] = G->arc[k][j]; //用新结点为起点的各边的权重替换weight[]中的值
                vex_index[j] = k; //更新候选边的起点索引，表示从k到j的边是新的候选边
            }
        }
    }
    return SUCCESS;
}
int main(void) {
    int ret;
    Mat_Graph G;
    ret=creategraph(&G);
    if (ret == SUCCESS) {
        prim(&G);
    } else {return ERROR;}

    return SUCCESS;
}