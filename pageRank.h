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
#include "list.h"
#include "invertedIndex.h"
#include "graph.h"
#include "readData.h"



List calculatePageRank(Graph g, float d, float diffPR, int maxIterations);

#endif /* pageRank_h */
