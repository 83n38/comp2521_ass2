// Graph ADT
// Adjacency List Representation ... COMP2521

#include "graph.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "list.h"

#define strEQ(g,t) (strcmp((g),(t)) == 0)

static int vertexID(char *, char **, int);


Graph newGraph(List L) {
    int nV = lengthLL(L);
    assert(nV >= 0);
    int i;
    
    Graph g = malloc(sizeof(GraphRep));
    assert(g != NULL);
    g->nV = nV;
    g->nE = 0;
    
    //add array of vertexs from list
    g->urls = malloc(nV*sizeof(char *));
    assert(g->urls != NULL);
    // allocate memory for array of lists
    g->edges = malloc(nV * sizeof(List));
    assert(g->edges != NULL);
    // allocate memory for page ranks
    g->ranks = malloc(nV * sizeof(float));
    assert(g->ranks != NULL);
    List curr = L;
    for (i = 0; i < nV; i++) {
        g->urls[i] = strdup(curr->url);
        g->edges[i] = NULL;
        curr = curr->next;
    }
    return g;
}

// check if vertex is valid in a graph
bool validV(Graph g, Vertex v) {
    return (g != NULL && v >= 0 && v < g->nV);
}

void insertEdge(Graph g, char *src, char *dest) {
    
    int v = vertexID(src, g->urls, g->nV);
    int w = vertexID(dest, g->urls, g->nV);
    
    assert(g != NULL && validV(g,v) && validV(g,w));
    
    if (!inLL(g->edges[v], dest)) {   // edge e not in graph
        g->edges[v] = insertLL(g->edges[v], dest, 0);
        g->nE++;
    }
}

/* I don't think we will need these for this assignment

void removeEdge(Graph g, Edge e) {
    assert(g != NULL && validV(g,e.v) && validV(g,e.w));
    
    if (inLL(g->edges[e.v], e.w)) {   // edge e in graph
        g->edges[e.v] = deleteLL(g->edges[e.v], e.w);
        g->edges[e.w] = deleteLL(g->edges[e.w], e.v);
        g->nE--;
    }
}
 
bool adjacent(Graph g, Vertex v, Vertex w) {
    assert(g != NULL && validV(g,v));
    
    return inLL(g->edges[v], w);
}
 
*/
 
void showGraph(Graph g) {
    assert(g != NULL);
    int i;
    
    printf("Number of vertices: %d\n", g->nV);
    printf("Number of edges: %d\n", g->nE);
    for (i = 0; i < g->nV; i++) {
        printf("%d - ", i);
        showLL(g->edges[i]);
    }
}


 
// vertexID(Str,urls,N)
// - searches for Str in array of urls[N]
// - returns index of Str if found, -1 if not
static int vertexID(char *str, char **urls, int N)
{
    int i;
    for (i = 0; i < N; i++)
        if (strEQ(str,urls[i])) return i;
    return -1;
}

void freeGraph(Graph g) {
    assert(g != NULL);
    int i;
    
    for (i = 0; i < g->nV; i++) {
        freeLL(g->edges[i]);
        free(g->urls[i]);
    }
    free(g->urls);
    free(g);
}

