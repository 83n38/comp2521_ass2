#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <math.h>
#include "readData.h"

List getCollection(void) {
 
    FILE *fp;
    fp = fopen("collection.txt", "r");
    if (fp == NULL) {
        perror("ERROR: ");
        return NULL;
    }
    List L = NULL;
    char url[10] = "\0";
    while(fscanf(fp, "%s", url) == 1) {
        L = insertLL(L, url);
    }
    fclose(fp);
    return L;
}

Graph getGraph(List L) {
    
    Graph G = newGraph(L);
    
    char *src;
    char *url;
    for(int i = 0; i < G->nV; i++) {
        
        src = strdup(G->urls[i]);
        url = strdup(G->urls[i]);
        strcat(url, ".txt");
        
        FILE *fp;
        fp = fopen(url, "r");
        if (fp == NULL) {
            perror("ERROR: ");
            return NULL;
        }
        int read = 1;
        char line[1000] = "\0";
        while (fgets(line, 1000, fp) != NULL && read) {
            
            if (strcmp(line, "#start Section-1\n") == 0) {
                //words unlikely to be more than 50 chars
                char dest[50] = "\0";
                while(fscanf(fp, "%s", dest) == 1
                      && strcmp(dest, "#end") != 0)
                {
                    insertEdge(G, src, dest);
                }
                free(src); free(url); read = 0;
            }
        }
        fclose(fp);
    }
    
    
    
    return G;
}

IList getInvertedList(List L) {
    
    //Need to finish
    return NULL;
}
