/*
 * 测试图的基本操作，包括创建图,拓扑排序算法。
*/
#include <stdio.h>
#include <stdlib.h>

#define MAXVEX 100
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
    G->numVertexes = 14;
    G->numEdges = 20;
    
    //初始化边
    for (int i=0; i<G->numVertexes; i++) {
        G->vex[i] = i;   //顶点为整形
        for (int j=0; j<G->numVertexes; j++) {
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
}

Status topological_sort(Adj_List_GraphPtr ALG) {
    EdgeNode *e;
    int curr;
    int k;

    //将所有入度为0的顶点入栈
    for (int i=0; i<ALG->numVertexes; i++) {
        if (ALG->adj_list[i].in == 0) {
            push(i);
        }
    }
    while(!is_empty()) {
        pop(&curr);
        printf("V%d -> ",ALG->adj_list[curr].data);
        e = ALG->adj_list[curr].head;
        while(e != NULL) {
            k = e->edge_vex;
            ALG->adj_list[k].in--;
            if (ALG->adj_list[k].in == 0) {
                push(k);
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
    
    ret=creategraph(&G);
    ret=create_adj_graph(G, &ALG); //&ALG的类型为Adj_Graph**
    ret=topological_sort(ALG);

    if (ret == SUCCESS) return SUCCESS;
}