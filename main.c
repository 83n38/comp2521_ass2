//
//  main.c
//  Comp2521 ass2
//
//  Created by Riley Vozzo on 8/10/18.
//  Copyright © 2018 Riley Vozzo. All rights reserved.
//

#include <stdio.h>
#include "readData.h"


int main(int argc, const char * argv[]) {
    // insert code here...
    printf("Hello, World!\n");
    
    List L = getCollection();
    Graph g = getGraph(L);
    
    showGraph(g);
    
    return 0;
}
