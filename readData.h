//
//  readData.h
//  Comp2521 ass2
//
//  Created by Riley Vozzo on 8/10/18.
//  Copyright © 2018 Riley Vozzo. All rights reserved.
//

#ifndef readData_h
#define readData_h

#include <stdio.h>
#include "invertedIndex.h"
#include "list.h"
#include "graph.h"
#include "myString.h"


List getCollection(void);
Graph getGraph(List L);
IList getInvertedList(List L);

#endif /* readData_h */
