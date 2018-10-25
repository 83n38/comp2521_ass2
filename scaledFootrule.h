//
//  scaledFootrule.h
//  COMP2521_assign2
//
//  Created by Ben Henderson on 24/10/18.
//  Copyright © 2018 Ben Henderson. All rights reserved.
//

#ifndef scaledFootrule_h
#define scaledFootrule_h

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <ctype.h>

#define EPSILON 0.00001

typedef struct url *Url;
typedef struct set *Set;

typedef struct url {
    char *name;     // a url has a name - "url23"
    int *ranks;     // and a array of ranks - ranks[0] would be the ranking in the first ranks file
    int ranksSize;  // size of the ranks array
    Set p;          // parent set
    Url next;       // pointer to another url
} url;

// dodgy implentation of a set
typedef struct set {
    int size;               // number of items in the set
    int *sizeInputRanks;    // array of the size of each input rank (same order as the ranks array in each url)
    int nRanks;             // number of input ranks
    Url head;               // first url in linked list
    Url *array;             // array of the urls in the set
} set;

Url inSet(Set C, char* u);              // returns Url if it is in the set, otherwise NULL
Set newSet(int n);                      // returns new empty set
Set addToSet(Set C, char* u, int rank); // returns the modified set

#endif /* scaledFootrule_h */
