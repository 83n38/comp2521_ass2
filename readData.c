#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "readData.h"

static void trim(char *str);

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
        L = insertLL(L, url); //'0' because no rank yet
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
    
    adjustNLinks(G); //prevents division by zero errors
    sumNeighborLinks(G); /* Calculates the sum of the incoming/outgoing
                          links of the neighbors of a all vertexs */
    
    return G;
}

IList getInvertedList(List L) {
    
    //Need to finish
    
    IList IL = newIList();
    
    char *src;
    char *url;
    
    for(List curr = L; curr != NULL; curr = curr->next) {
        
        src = strdup(curr->url);
        url = strdup(curr->url);
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
            
            if (strcmp(line, "#start Section-2\n") == 0) {
                //words unlikely to be more than 100 chars
                char word[100] = "\0";
                while(fscanf(fp, "%s", word) == 1
                      && strcmp(word, "#end") != 0)
                {
                    trim(word);
                    IList wordNode = IListSearch(IL, word);
                    if (wordNode == NULL) {
                        IL = IListInsert(IL, word);
                        wordNode = IListSearch(IL, word); // sometimes crashes here
                        // For some reason wordNode is sometimes NULL depite the word being in the IList
                    }
                    if (!inLL(wordNode->urlList, src)) {
                        wordNode->urlList = insertLL(wordNode->urlList, src);
                    }
                }
                free(src); free(url); read = 0;
            }
        }
        fclose(fp);
    }
    return IL;
}

/* remove leading/trailing spaces from a string, as well as
   '.' (dot), ',' (comma), ';' (semicolon), ? (question mark)
   characters if at end of word.
   P.S. Code is modifed from starting code in COMP1521 Ass02
*/
static void trim(char *str) {
    int first, last;
    first = 0;
    while (isspace(str[first])) first++;
    
    last  = strlen(str)-1;
    while (isspace(str[last]) || str[last] == '.' || str[last] == '?' ||
           str[last] == ';' || str[last] == ',') {
        last--;
    }
    int i, j = 0;
    for (i = first; i <= last; i++) {
        str[i] = tolower(str[i]);
        str[j++] = str[i];
    }
    str[j] = '\0';
}
