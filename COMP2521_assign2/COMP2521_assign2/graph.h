// Graph ADT interface ... COMP2521
#include <stdbool.h>
#include "list.h"

typedef struct GraphRep *Graph;

// vertices are ints
typedef int Vertex;

typedef struct GraphRep {
    char **urls;  // array of urls (strings)
    List *edges;  // array of lists
    int   nV;     // #vertices
    int   nE;     // #edges
} GraphRep;


Graph newGraph(List L);
void  insertEdge(Graph, char *src, char *dest);
//void  removeEdge(Graph, Edge);
//bool  adjacent(Graph, Vertex, Vertex);
void  showGraph(Graph);
void  freeGraph(Graph);
