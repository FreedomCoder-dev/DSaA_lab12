#include <stdio.h>
#include <limits.h>
#include "graph_reader.h"
#include "graph_adapter.h"
#include "queue.h"


int arr_first_empty(int *arr, int count) {
    int id = 0;
    while (*arr++ && count--) ++id;
    return id;
}

void printAdjMatrix(struct TGAdjacencyMatrix *mtx) {
    printf("%d\n", mtx->node_count);
    int *mtx_end = mtx->mtx_data + mtx->node_count * mtx->node_count;
    for (int *i = mtx->mtx_data; i < mtx_end; ++i) {
        if ((i != mtx->mtx_data) && (i - mtx->mtx_data) % mtx->node_count == 0)
            printf("\n");
        printf("%d ", *i);
    }
    printf("\n");
}


void printEdgeList(struct TGEdgeList *edges) {
    struct TGEdge *curr = edges->edges;
    printf("%d\n", edges->edge_count);
    for (int i = 0; i < edges->edge_count; ++i) {
        printf("%d %d %d \n", curr->node1, curr->node2, curr->weight);
        curr++;
    }
}

void dijkstra_compute(struct TGAdjacencyMatrix *mtx, int *routeLengths, int *escapeRoutes) {
    int (*matrix)[mtx->node_count] = (int (*)[mtx->node_count]) mtx->mtx_data;
    int *visited = malloc(sizeof(int) * mtx->node_count);
    int minNode = 0;

    for (int i = 0; i < mtx->node_count; ++i) {
        int min = INT_MAX;

        for (int j = 0; j < mtx->node_count; ++j)
            if (!visited[j] && routeLengths[j] < min) {
                min = routeLengths[j];
                minNode = j;
            }

        visited[minNode] = 1;

        for (int j = 0; j < mtx->node_count; ++j)
            if (!visited[j] && matrix[minNode][j] && routeLengths[minNode] < INT_MAX &&
                routeLengths[minNode] + matrix[minNode][j] < routeLengths[j]) {
                routeLengths[j] = routeLengths[minNode] + matrix[minNode][j];
                escapeRoutes[j] = minNode;
            }
    }

    free(visited);
}

int main() {
    struct TGEdgeList *edges, *e2;

    FILE *in = fopen("../in.txt", "r");
    edges = readTGEdgeList(in);
    fclose(in);
    struct TGAdjacencyMatrix *mtx = constructTGAMatrixOriented(edges);
//    printEdgeList(edges);
//    printAdjMatrix(mtx);

    const int START_VERTEX = 0;

    int *routeLengths = malloc(sizeof(int) * mtx->node_count);
    for (int i = 0; i < mtx->node_count; ++i) {
        routeLengths[i] = INT_MAX;
    }
    routeLengths[START_VERTEX] = 0;
    int *escapeRoutes = malloc(sizeof(int) * mtx->node_count);
    for (int i = 0; i < mtx->node_count; ++i) {
        escapeRoutes[i] = START_VERTEX;
    }

    dijkstra_compute(mtx, routeLengths, escapeRoutes);

    for (int i = 0; i < mtx->node_count; ++i) {
        printf("Vertex %d: route length %d, %d", i+1, routeLengths[i], i+1);
        int cVert = i;
        while (cVert != START_VERTEX) printf(" <- %d", (cVert = escapeRoutes[cVert]) + 1);
        printf("\n");
    }

//    printAdjMatrix(mtx);

    return 0;
}
