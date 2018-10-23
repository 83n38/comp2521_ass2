// Linked list interface ... COMP2521

#ifndef list_h
#define list_h

#include <stdbool.h>
#include "myString.h"

typedef struct Node *List;

typedef struct Node {
    char*        url;
    double       rank; //may not be used
    int          nOut; //may not be used
    int          matchCount;
    int          nWords;
    float        tf_idf;
    struct Node *next;

} Node;

List insertLL(List, char *url);
List deleteLL(List, char *url);
List inLL(List, char *url);
void freeLL(List);
void showLL(List);
int lengthLL(List);
void bubbleSort(List head, int (*compare)(Node*, Node*));

// comparison functions (to pass into bubble sort)
int compareRanks(Node *a, Node *b);
int compareMatches(Node *a, Node *b);
int compareTf_idf(Node *a, Node *b);

#endif
