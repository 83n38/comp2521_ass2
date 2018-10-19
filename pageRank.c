//
//  pageRank.c
//  Ass2
//
//  Created by Riley Vozzo on 15/10/18.
//  Copyright © 2018 Riley Vozzo. All rights reserved.
//

#include "pageRank.h"


int compareRanks(Node *a, Node *b);

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
    int (*compareRanksPtr)(Node *, Node *);
    compareRanksPtr = &compareRanks;
    bubbleSortByRank(pageRankList, &compareRanks);
    
    FILE *fp;
    fp = fopen("pagerankList.txt", "w");
    if(fp == NULL) {
        perror("Error: ");
    }
    
    for (List curr = pageRankList; curr != NULL; curr = curr->next) {
        fprintf(fp, "%s, %d, %.7f\n", curr->url, curr->nOut, curr->rank);
    }
    
    
    //showGraph(g);
    
    
    return 0;
}


List getPageRankList(Graph g, double d, double diffPR, int maxIterations) {
   
    
    //double damp = (((double)1-d)/(double)g->nV) + d; //calc only one because constant
    
    for(int page = 0; page < g->nV; page++) {
        g->ranks[page] = (double)1/(double)g->nV;
    }
    
    int iteration = 0;
    double diff = diffPR;
    while(iteration < maxIterations && diff >= diffPR) {
       
        /*
        printf("Iteration %d\n", iteration);
        
        for (int v = 0; v < g->nV; v++) {
            printf("%s: %.7f\n",g->urls[v] ,g->ranks[v]);
        }
        printf("\n");
        */
        
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

void calculatePageRanks(Graph g, double d) {
    
    for(int w = 0; w < g->nV; w++) {
        double sum = 0;
        for(int v = 0; v < g->nV; v++) {
            if(inLL(g->edges[v], g->urls[w]) && v != w) {
                //printf("%s -> %s\n", g->urls[v], g->urls[w]);
                double Win = g->nInLinks[w]/g->sumNeighboursInLinks[v];
                double Wout = g->nOutLinks[w]/g->sumNeighboursOutLinks[v];
                sum += g->ranks[v] * Win * Wout;
            }
        }
        g->newRanks[w] = (((double)1-d)/(double)g->nV) + d * sum;
    }
}

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

//if a < b returns negative
//if a == b returns zero
//if a > b returns positive
int compareRanks(Node *a, Node *b) {
    if(a->rank - b->rank == 0) {
        return 0;
    } if (a->rank - b->rank > 0) {
        return 1;
    } //if (a->rank - b->rank < 0) {
    return -1;
    //}
}

