//
//  pageRank.c
//  Ass2
//
//  Created by Riley Vozzo on 15/10/18.
//  Copyright © 2018 Riley Vozzo. All rights reserved.
//

#include "pageRank.h"

int main(int argc, const char * argv[]) {
    // insert code here...
    printf("Starting in main!\n");
    /*
    if (argc != 4) {
        fprintf(stderr, "Not enough arguments");
        return 1;
    }
    float d = atof(argv[1]);
    float diffPR = atof(argv[2]);
    int maxIterations = atoi(argv[3]);
    */
    List L = getCollection();
    Graph g = getGraph(L);
    showGraph(g);
    
    
    return 0;
}

List getPageRankList(Graph g, float d, float diffPR, int maxIterations) {
   
    
    float damp = ((1-d)/(float)g->nV)* d; //calc only one because constant
    
    for(int page = 0; page < g->nV; page++) {
        g->ranks[page] = 1/g->nV;
    }
    
    int iteration = 0;
    float diff = diffPR;
    while(iteration < maxIterations && diff >= diffPR) {
        iteration++;
        calculatePageRanks(g, damp, diffPR, maxIterations);
        diff = calculateDiff(g);
        savePageRanks(g);
    }
    
    List pageRanks = NULL;
    for(int page = 0; page < g->nV; page++) {
        pageRanks = insertLL(pageRanks, g->urls[page]);
        List node = inLL(pageRanks, g->urls[page]);
        node->rank = g->ranks[page];
    }
    return pageRanks;
}

void calculatePageRanks(Graph g, float damp, float diffPR, int maxIterations) {
    
    for(int w = 0; w < g->nV; w++) {
        float sum = 0;
        for(int v = 0; v < g->nV; v++) {
            if(inLL(g->edges[v], g->urls[w])) {
                float Win = g->nInLinks[w]/g->sumNeighboursInLinks[v];
                float Wout = g->nOutLinks[w]/g->sumNeighboursOutLinks[v];
                sum += g->ranks[v] * Win * Wout;
            }
        }
        g->newRanks[w] = damp * sum;
    }
}

float calculateDiff(Graph g) {
    float diff = 0;
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
