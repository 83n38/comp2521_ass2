// Linked list interface ... COMP2521

#ifndef list_h
#define list_h

#include <stdbool.h>

typedef struct Node *List;

typedef struct Node {
    char*        url;
    float        rank;
    struct Node *next;
} Node;

List insertLL(List, char *url, float rank);
List deleteLL(List, char *url);
bool inLL(List, char *url);
void freeLL(List);
void showLL(List);
int lengthLL(List);

#endif
