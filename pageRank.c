//
//  pageRank.c
//  Ass2
//
//  Created by Riley Vozzo on 15/10/18.
//  Copyright © 2018 Riley Vozzo. All rights reserved.
//

#include "pageRank.h"

static void bubbleSortPageRankList(List head);
static void swap(Node *a, Node *b);



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
    
    
    //float d = 0.85;
    //float diffPR = 0.00001;
    //int maxIterations = 10;
    
    List L = getCollection();
    Graph g = getGraph(L);
    List pageRankList = getPageRankList(g, d, diffPR, maxIterations);
    bubbleSortPageRankList(pageRankList);
    for (List curr = pageRankList; curr != NULL; curr = curr->next) {
        printf("%s, %d, %.7f\n", curr->url, curr->nOut, curr->rank);        
    }
    
    
    //showGraph(g);
    
    
    return 0;
}

List getPageRankList(Graph g, float d, float diffPR, int maxIterations) {
   
    
    float damp = (((float)1-d)/(float)g->nV) + d; //calc only one because constant
    
    for(int page = 0; page < g->nV; page++) {
        g->ranks[page] = (float)1/(float)g->nV;
    }
    
    int iteration = 0;
    float diff = diffPR;
    while(iteration < maxIterations && diff >= diffPR) {
        iteration++;
        
        /* for testing
        
        for (int v = 0; v < g->nV; v++) {
            printf("%.7f ", g->ranks[v]);
        }
        printf("\n");
         
         */
        
        calculatePageRanks(g, damp, diffPR, maxIterations);
        diff = calculateDiff(g);
        savePageRanks(g);
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

/* Bubble sort a given pageRanklist by rank */
static void bubbleSortPageRankList(List head) {
    int swapped = 1;
    Node *curr;
    Node *end = NULL;
    
    /* Checking for empty list */
    if (head == NULL) {
        fprintf(stderr, "pageRankList is empty");
        return;
    }
    while (swapped) {
        swapped = 0;
        curr = head;
        while (curr->next != end)
        {
            if (curr->rank < curr->next->rank)
            {
                swap(curr, curr->next);
                swapped = 1;
            }
            curr = curr->next;
        }
        end = curr;
    }
}

/* function to swap data of two pageRankList nodes a and b*/
static void swap(Node *a, Node *b) {
    char *tempUrl = a->url;
    int tempNout = a->nOut;
    float tempRank = a->rank;
    a->url = b->url;
    a->nOut = b->nOut;
    a->rank = b->rank;
    b->url = tempUrl;
    b->nOut = tempNout;
    b->rank = tempRank;
}
