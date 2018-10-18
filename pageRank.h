//
//  pageRank.h
//  Ass2
//
//  Created by Riley Vozzo on 15/10/18.
//  Copyright © 2018 Riley Vozzo. All rights reserved.
//

#ifndef pageRank_h
#define pageRank_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "list.h"
#include "invertedIndex.h"
#include "graph.h"
#include "readData.h"



void calculatePageRanks(Graph g, double d);
List getPageRankList(Graph g, double d, double diffPR, int maxIterations);
double calculateDiff(Graph g);
void savePageRanks(Graph g);

#endif /* pageRank_h */
