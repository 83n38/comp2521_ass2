//
//  searchPagerank.c
//  COMP2521_assign2
//
//  Created by Ben Henderson on 18/10/18.
//  Copyright © 2018 Ben Henderson. All rights reserved.
//

#include "searchPagerank.h"
/*
int main(int argc, const char * argv[]) {
    // insert code here...
    if (argc == 1) {
        printf("Usage: ./%s <search term> <search term>...", argv[0]);
        return 0;
    }
    int nTerms = argc - 1;
    char **searchTerms = malloc(nTerms * sizeof(char*));
    int i = 1;
    for (printf("Search terms: "); i <= nTerms; i++) {
    
        searchTerms[i-1] = strdup(argv[i]);
        printf("%s, ", argv[i]);
        
    }
    
    // get a linked list of the urls that matched, and count how many matches
    List matched_Url_list = findMatchedUrls(searchTerms, nTerms);
    
    // add their corresponding page rank
    addRanks(matched_Url_list);
    
    // init function pointer
    int (*comparePtr)(Node *, Node *);
    // sort by ranks
    comparePtr = &compareRanks;
    bubbleSort(matched_Url_list, comparePtr);
    
    // now sort by matches
    comparePtr = &compareMatches;
    bubbleSort(matched_Url_list, comparePtr);
    
    for (int i = 0; i < nTerms; i++) {
        free(searchTerms[i]);
    }
    free(searchTerms);
    for (List curr = matched_Url_list; curr != NULL; curr = curr->next) {
        printf("%s: nMatch %d  rank %lf\n", curr->url, curr->matchCount, curr->rank);
    }
    
    
    return 0;
}
*/
List findMatchedUrls(char *searchTerms[], int nTerms) {
    
    List matched_Url_list = NULL;

    // open invertedIndex.txt
    FILE* fp;
    fp = fopen("invertedIndex.txt", "r");
    if (fp == NULL) {
        printf("File not found");
        return 0;
    }
    
    // reads the file line by line
    char line[1000] = "\0";
    
    while (fgets(line, 1000, fp) != NULL) {
        char *word;
        int urlCount = 0;
        int wordsMatched = 0;
        char *line2 = strdup(line);
        while ((word = strsep(&line2, " ")) != NULL) {
            // check to see if the word is one of our search terms
            if (urlCount == 0) {
                // then see if this is a search term
                for (int i = 0; i < nTerms; i++ ) {
                    if(strcmp(word, searchTerms[i]) == 0) {
                        // we found the search term in the invertedIndex
                        wordsMatched++;
                        printf("found a match\n");
                        
                    }
                }
            } else if (wordsMatched && word[0] == 'u') {
                // add this current url to the linked list
                Node *url = inLL(matched_Url_list, word);
                if(url != NULL) {
                    url->matchCount++;
                } else {
                    matched_Url_list = insertLL(matched_Url_list, word);
                    url = inLL(matched_Url_list, word);
                    url->matchCount = 1;
                }
            }
            
            urlCount++;
            
        }
        free(line2);
    }
    
    
    return matched_Url_list;
}

void addRanks(List matched_Urls) {
    
    FILE* fp;
    fp = fopen("pagerankList.txt", "r");
    if (fp == NULL) {
        printf("File not found");
        return;
    }
    
    char line[1000] = "\0";
    
    while (fgets(line, 1000, fp) != NULL) {
        char word[50];
        int nOut;
        double rank;
        if (sscanf(line, "%50[^,], %d, %lf\n", word, &nOut, &rank)) {
            // check to see if the word is one of our search terms
            Node *url = inLL(matched_Urls, word);
            if(url != NULL) {
                url->rank = rank;
            }
        }
    }
}


