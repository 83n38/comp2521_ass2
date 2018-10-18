//
//  searchPagerank.c
//  COMP2521_assign2
//
//  Created by Ben Henderson on 18/10/18.
//  Copyright © 2018 Ben Henderson. All rights reserved.
//

#include "searchPagerank.h"
#include "readData.h"

int main(int argc, const char * argv[]) {
    // insert code here...
    if (argc == 1) {
        printf("Usage: ./%s <search term> <search term>...", argv[0]);
    }
    
    printf("Search terms: ");
    for (int i = 1; i < argc; i++) {
        printf("%s, ", argv[i]);
    }
    
    List L = getCollection();
    Graph g = getGraph(L);
    IList IL = getInvertedList(L);
    showGraph(g);
    
    return 0;
}
