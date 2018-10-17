// Graph ADT interface ... COMP2521

#ifndef graph_h
#define graph_h

#include <stdbool.h>
#include "list.h"

typedef struct GraphRep *Graph;

// vertices are ints
typedef int Vertex;

typedef struct GraphRep {
    char **urls;        // array of urls (strings)
    float *ranks;       // array of floats
    float *newRanks;    // ""
    float *nInLinks;    // ""
    float *nOutLinks;   // ""
    float *sumNeighboursInLinks;
    float *sumNeighboursOutLinks;
    List *edges;        // array of lists
    int   nV;           // #vertices
    int   nE;           // #edges
} GraphRep;


Graph newGraph(List L);
void  insertEdge(Graph, char *src, char *dest);
//void  removeEdge(Graph, Edge);
//bool  adjacent(Graph, Vertex, Vertex);
void  showGraph(Graph);
void  freeGraph(Graph);
void calculateWeights(Graph g);
void sumNeighborLinks(Graph g);
void adjustNLinks(Graph g);


#endif

