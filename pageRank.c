//
//  pageRank.c
//  Ass2
//
//  Created by Riley Vozzo on 15/10/18.
//  Copyright © 2018 Riley Vozzo. All rights reserved.
//

#include "pagerank.h"


int compareRanks(Node *a, Node *b);

/*
int main(int argc, const char * argv[]) {
 
    if (argc != 4) {
        fprintf(stderr, "Not enough arguments");
        return 1;
    }
    double d = atof(argv[1]);
    double diffPR = atof(argv[2]);
    int maxIterations = atoi(argv[3]);

    List L = getCollection();
    Graph g = getGraph(L);

    List pageRankList = getPageRankList(g, d, diffPR, maxIterations);
    bubbleSort(pageRankList, &compareRanks);
    
    FILE *fp;
    fp = stdin;
 
    fp = fopen("pagerankList.txt", "w");
    if(fp == NULL) {
        perror("Error: ");
    }
    
    for (List curr = pageRankList; curr != NULL; curr = curr->next) {
        fprintf(fp, "%s, %d, %.7f\n", curr->url, curr->nOut, curr->rank);
    }
    
    freeLL(L);
    freeGraph(g);
    
    return 0;
}
*/

/* Gets a graph and returns a LL of urls with their page ranks*/
List getPageRankList(Graph g, double d, double diffPR, int maxIterations) {
   
    for(int page = 0; page < g->nV; page++) {
        g->ranks[page] = (double)1/(double)g->nV;
    }
    
    int iteration = 0;
    double diff = diffPR;
    while(iteration < maxIterations && !(diff < diffPR)) {
        
        calculatePageRanks(g, d);
        diff = calculateDiff(g);
        savePageRanks(g);
        iteration++;
    }
    
    List pageRanks = NULL;
    for(int page = 0; page < g->nV; page++) {
        pageRanks = insertLL(pageRanks, g->urls[page]);
        List node = inLL(pageRanks, g->urls[page]);
        node->rank = g->ranks[page];
        node->nOut = lengthLL(g->edges[page]);
    }
    return pageRanks;
}

/* Calculates the page ranks for the urls in a Graph */
void calculatePageRanks(Graph g, double d) {
    
    for(int w = 0; w < g->nV; w++) {
        double sum = 0;
        for(int v = 0; v < g->nV; v++) {
            if(inLL(g->edges[v], g->urls[w]) && v != w) {
                double Win = g->nInLinks[w]/g->sumNeighboursInLinks[v];
                double Wout = g->nOutLinks[w]/g->sumNeighboursOutLinks[v];
                sum += g->ranks[v] * Win * Wout;
            }
        }
        g->newRanks[w] = (((double)1-d)/(double)g->nV) + d * sum;
    }
}

/* Calculates the difference in page ranks between iterations */
double calculateDiff(Graph g) {
    double diff = 0;
    for(int page = 0; page < g->nV; page++) {
        diff += fabs(g->newRanks[page] - g->ranks[page]);
    }
    return diff;
}

/* Moves ranks in newRanks array to ranks array in graph*/
void savePageRanks(Graph g) {
    for(int page = 0; page < g->nV; page++) {
        g->ranks[page] = g->newRanks[page];
    }
}


