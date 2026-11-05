/*
 * 测试图的基本操作，包括创建图、关键路径算法。
*/
#include <stdio.h>
#include <stdlib.h>

#define MAXVEX 100
#define MAX 0x7fffffff
#define MAXEDGE MAXVEX * MAXVEX
#define SUCCESS 0
#define ERROR 1

typedef int Status;
typedef int VertexType;
typedef int EdgeType;
typedef enum {false, true} Bool;

typedef struct {
    VertexType vex[MAXVEX];
    EdgeType arc[MAXVEX][MAXVEX];
    int numVertexes, numEdges;
} Mat_Graph;

typedef struct enode {
    int edge_vex;
    int weight;//增加了边的权重
    struct enode* next;
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
    int numVertexes;
    int numEdges;
} Adj_Graph;//邻接表表示的图. 每个顶点都在图中，每个顶点的邻接表存储了与该顶点相连的所有边。

typedef Adj_Graph* Adj_List_GraphPtr; //Adj_List_GraphPtr is a pointer to an Adj_Graph structure, 
                                      //representing a graph using an adjacency list.

int top=-1;
int stack[MAXVEX]; //用于存储入度为0的顶点

Status push(int v) {
    if (top >= MAXVEX - 1) {
        printf("Stack overflow\n");
        return ERROR;
    }
    top++;
    stack[top] = v;
    return SUCCESS;
}

Status pop(int *v) {
    if (top < 0) {
        printf("Stack underflow\n");
        return ERROR;
    }
    *v = stack[top];
    top--;
    return SUCCESS;
}

Bool is_empty() {
    return (top < 0) ? true : false;
}

Status creategraph(Mat_Graph *G) {
    G->numVertexes = 10;
    G->numEdges = 13;
    
    //初始化边
    for (int i=0; i<G->numVertexes; i++) {
        G->vex[i] = i;   //顶点为整形
        for (int j=0; j<G->numVertexes; j++) {
            if (i==j)
            {
               G->arc[i][j] = 0;
            } else {
                G->arc[i][j] = MAX;
            }
        }
    }

    //设置边及权值
    G->arc[0][1]=3;
    G->arc[0][2]=4;

    G->arc[1][3]=5;
    G->arc[1][4]=6;

    G->arc[2][3]=8;
    G->arc[2][5]=7;

    G->arc[3][4]=3;

    G->arc[4][6]=9;
    G->arc[4][7]=4;

    G->arc[5][7]=6;

    G->arc[6][9]=2;

    G->arc[7][8]=5;

    G->arc[8][9]=3;

    return SUCCESS;
}

Status create_adj_graph(Mat_Graph G, Adj_Graph **ALG) {
    *ALG = (Adj_Graph*)malloc(sizeof(Adj_Graph));
    (*ALG)->numVertexes = G.numVertexes;
    (*ALG)->numEdges = G.numEdges;
    
    //创建邻接表表示的图
    for (int i=0; i<G.numVertexes; i++) {
        (*ALG)->adj_list[i].data = G.vex[i];
        (*ALG)->adj_list[i].in = 0;
        (*ALG)->adj_list[i].head = NULL;
    }
    for (int i=0; i<G.numVertexes; i++) {
        for (int j=0; j<G.numVertexes; j++) {
            if (G.arc[i][j] !=0 && G.arc[i][j] < MAX) { //i->j有连接
                EdgeNode* edge = (EdgeNode*)malloc(sizeof(EdgeNode));
                edge->edge_vex = j;
                edge->weight = G.arc[i][j];
                edge->next = (*ALG)->adj_list[i].head; //头插法插入顶点i
                (*ALG)->adj_list[i].head = edge;
                (*ALG)->adj_list[j].in++; //顶点j增加入度
            }
        }
    }
    return SUCCESS;
}

Status critical_path(Adj_List_GraphPtr ALG) {
    EdgeNode *e;
    int top=-1;
    int top2=-1;
    int stack[MAXVEX]; //用于存储拓扑排序的顶点,计算最早发生时间
    int stack2[MAXVEX]; //用于存储逆拓扑排序的顶点，计算最晚发生时间
    int etv[MAXVEX]; //用于存储每个事件的最早发生时间
    int ltv[MAXVEX]; //用于存储每个事件的最晚发生时间
    int curr;
    int k;

    for(int i=0; i<ALG->numVertexes; i++) {
        if (ALG->adj_list[i].in == 0) {
            top++;
            stack[top] = i;
        }
    }
    
    //初始化etv数组
    for(int i=0; i<ALG->numVertexes; i++) {
        etv[i] = 0;
    }

    while(top != -1) {
        //从stack出栈
        curr = stack[top];
        top--;
        printf("V%d -> ",ALG->adj_list[curr].data);

        //进栈stack2，用于逆拓扑排序
        top2++;
        stack2[top2]=curr;

        e = ALG->adj_list[curr].head;
        while(e != NULL) {
            k = e->edge_vex;
            if (etv[k] < etv[curr] + e->weight) {
                etv[k] = etv[curr] + e->weight;
            }
            ALG->adj_list[k].in--;
            if (ALG->adj_list[k].in == 0) {
                top++;
                stack[top] = k;
            }
            e = e->next;
        }
    }
    printf("End\n");
    printf("etv: ");
    for(int i=0; i<ALG->numVertexes; i++) {
        printf("V%d:%d ", ALG->adj_list[i].data, etv[i]);
    }
    printf("\n");

    //初始化ltv数组
    for(int i=0; i<ALG->numVertexes; i++) {
        ltv[i] = etv[ALG->numVertexes-1]; //初始化ltv数组，所有事件的最晚发生时间等于最后一个事件的最早发生时间
    }
    //计算每个事件的最晚发生时间
    while(top2 != -1) {
        curr = stack2[top2];
        top2--;
        e = ALG->adj_list[curr].head;
        while(e != NULL) {
            k = e->edge_vex;
            if (ltv[k] - e->weight < ltv[curr]) {
                ltv[curr] = ltv[k] - e->weight;
            }
            e = e->next;
        }
    }
    printf("ltv: ");
    for(int i=0; i<ALG->numVertexes; i++) {
        printf("V%d:%d ", ALG->adj_list[i].data, ltv[i]);
    }
    printf("\n");

    //critical path
    printf("critical path: ");
    for(int i=0; i<ALG->numVertexes; i++) {
        if (etv[i] == ltv[i]) {
            printf("V%d ->", ALG->adj_list[i].data);
        }
    }
    printf("End\n");
    return SUCCESS;
}

int main(void) {
    int ret;
    Mat_Graph G;  //邻接矩阵表示的图
    Adj_List_GraphPtr ALG=NULL;  //邻接表表示的图，ALG是指针，它指向Adj_Graph结构体.
    
    ret=creategraph(&G);
    ret=create_adj_graph(G, &ALG); //&ALG的类型为Adj_Graph**
    ret=critical_path(ALG);

    if (ret == SUCCESS) return SUCCESS;
}