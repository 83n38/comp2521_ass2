// Linked list interface ... COMP2521

#ifndef list_h
#define list_h

#include <stdbool.h>

typedef struct Node *List;

typedef struct Node {
    char*        url;
    double        rank; //may not be used
    int          nOut; //may not be used
    int          matchCount;
    struct Node *next;
} Node;

List insertLL(List, char *url);
List deleteLL(List, char *url);
List inLL(List, char *url);
void freeLL(List);
void showLL(List);
int lengthLL(List);
void bubbleSortByRank(List head, int (*compare)(Node*, Node*));

#endif
