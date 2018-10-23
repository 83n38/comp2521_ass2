// Binary Search IList ADT interface ... COMP2521

#ifndef invertedIndex_h
#define invertedIndex_h

#include <stdbool.h>
#include "list.h"

typedef char* Word;      // Word is just a key

typedef struct BSTNode *IList;

// I don't know whether this should be in invertedIndex.h (shouldn't it be hidden as its an ADT?)
typedef struct BSTNode {
    char *word;
    List urlList;
    IList left, right;
    float idf;
} BSTNode;

IList newIList(void);        // create an empty IList
void freeIList(IList);   // free memory associated with IList
void showIList(IList);   // display a IList (sideways)

IList IListSearch(IList, Word);   // check whether a Word is in a IList
int  IListHeight(IList);         // compute height of IList
int  IListNumNodes(IList);       // count #nodes in IList
IList IListInsert(IList, Word);   // insert a new Word into a IList
IList IListDelete(IList, Word);   // delete a Word from a IList
void outputIL(IList t, FILE *fp);
IList calculate_idf(IList t, int nPages);

void whiteBoxTests(void);           // tests invertedIndex ADT

// internal functions made visible for testing
IList rotateRight(IList);
IList rotateLeft(IList);
IList insertAtRoot(IList, Word);
IList insertRandom(IList, Word);
IList insertAVL(IList, Word);
IList partition(IList, int);
IList rebalance(IList);

#endif
