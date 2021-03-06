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
    // allocate memory for page ranks/newRanks
    g->ranks = malloc(nV * sizeof(double));
    assert(g->ranks != NULL);
    g->newRanks = malloc(nV * sizeof(double));
    assert(g->newRanks != NULL);
    // allocate memory for Nin/outLinks
    g->nInLinks = malloc(nV * sizeof(double));
    assert(g->nInLinks != NULL);
    g->nOutLinks = malloc(nV * sizeof(double));
    assert(g->nOutLinks != NULL);
    //allocate memory for sumNeighboursIn/OutLinks
    g->sumNeighboursInLinks = malloc(nV * sizeof(double));
    assert(g->sumNeighboursInLinks != NULL);
    g->sumNeighboursOutLinks = malloc(nV * sizeof(double));
    assert(g->sumNeighboursOutLinks != NULL);

    List curr = L;
    for (i = 0; i < nV; i++) {
        g->urls[i] = mystrdup2(curr->url);
        g->edges[i] = NULL;
        g->ranks[i] = 0;
        g->nInLinks[i] = g->nOutLinks[i] = 0;
        g->sumNeighboursInLinks[i] = g->sumNeighboursOutLinks[i] = 0;
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
        if (v != w) { //only add in/outLink if not self loop
            g->edges[v] = insertLL(g->edges[v], dest);
            g->nInLinks[w]++;
            g->nOutLinks[v]++;
        }
        g->nE++;
    }
}

void showGraph(Graph g) {
    assert(g != NULL);
    int i;
    
    printf("Number of vertices: %d\n", g->nV);
    printf("Number of edges: %d\n", g->nE);
    for (i = 0; i < g->nV; i++) {
        printf("%s: %d - ", g->urls[i], i);
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
    free(g->edges);
    free(g->ranks);
    free(g->newRanks);
    free(g->nInLinks);
    free(g->nOutLinks);
    free(g->sumNeighboursInLinks);
    free(g->sumNeighboursOutLinks);    
    free(g);
}


/* Changes number of outgoing Links to 0.5
   if zero in order to avoid division by zero errors */
void adjustNLinks(Graph g) {
    for(int v = 0; v < g->nV; v++) {
        if(g->nOutLinks[v] == 0) { g->nOutLinks[v] = 0.5; }
    }
}

/* Calculates the sum of the incoming/outgoing links of the neighbors
 of a vertex */
void sumNeighborLinks(Graph g) {
    
    for(int v = 0; v < g->nV; v++) {
        for (List curr = g->edges[v]; curr != NULL; curr = curr->next) {
            int w = vertexID(curr->url, g->urls, g->nV);
            g->sumNeighboursOutLinks[v] += g->nOutLinks[w];
            g->sumNeighboursInLinks[v] += g->nInLinks[w];
        }
    }
}

