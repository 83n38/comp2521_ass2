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
    
    if (argc != 4) {
        fprintf(stderr, "Not enough arguments");
        return 1;
    }
    
    float d = atof(argv[1]);
    float diffPR = atof(argv[2]);
    int maxIterations = atoi(argv[3]);
    List L = getCollection();
    Graph g = getGraph(L);
    
    
    
    return 0;
}

List calculatePageRank(Graph g, float d, float diffPR, int maxIterations) {
    
    for(int page = 0; page < g->nV; page++) {
        g->ranks[page] = 1/g->nV;
    }
    
    int iteration = 0;
    float diff = diffPR;
    
    while(iteration < maxIterations && diff >= diffPR) {
        iteration++;

        
        
        
    }
    
    List pageRanks = NULL;
    for(int page = 0; page < g->nV; page++) {
        pageRanks = insertLL(pageRanks, g->urls[page], g->ranks[page]);
    }
    return pageRanks;
}
