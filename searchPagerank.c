//
//  searchPagerank.c
//  COMP2521_assign2
//
//  Created by Ben Henderson on 18/10/18.
//  Copyright © 2018 Ben Henderson. All rights reserved.
//

#include "searchPagerank.h"
#include "readData.h"
#include "list.h"

int main(int argc, const char * argv[]) {
    // insert code here...
    if (argc == 1) {
        printf("Usage: ./%s <search term> <search term>...", argv[0]);
        return 0;
    }
    
    int i = 1;
    for (printf("Search terms: "); i < argc - 1; i++) {
        
        printf("%s, ", argv[i]);
    }
    printf("%s\n", argv[i]);
    
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
        char word[50] = "\0";
        int wordCount = 0;

        while (sscanf(line, "%s", word) == 1) {
            // check to see if the word is one of our search terms
            if (wordCount == 0) {
                // then see if this is a search term
                int i = 1;
                for (char* searchTerm = argv[i]; i < argc - 1; searchTerm = argv[++i] ) {
                    if(strcmp(word, searchTerm) == 0) {
                        // we found the search term in the invertedIndex
                        printf("found a match\n");
                        
                    }
                }
            }
            
            wordCount++;
            
        }
    }
    
    
