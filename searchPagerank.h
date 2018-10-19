//
//  searchPagerank.h
//  COMP2521_assign2
//
//  Created by Ben Henderson on 18/10/18.
//  Copyright © 2018 Ben Henderson. All rights reserved.
//

#ifndef searchPagerank_h
#define searchPagerank_h

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "list.h"
#include "readData.h"

List findMatchedUrls(char** searchTerms, int nTerms);
void addRanks(List matched_Urls);
int compareRanks(Node *a, Node *b);

#endif /* searchPagerank_h */
