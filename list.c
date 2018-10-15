// Linked list implementation ... COMP2521
#include "list.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>



Node *makeNode(char *url, float rank) {
    Node *new = malloc(sizeof(Node));
    assert(new != NULL);
    new->url = strdup(url);
    new->rank = rank;
    new->next = NULL;
    return new;
}

List insertLL(List L, char *url, float rank) {
    if (inLL(L, url))
        return L;
    
    // add new node at the beginning
    Node *new = makeNode(url, rank);
    new->next = L;
    return new;
}

List deleteLL(List L, char *url) {
    if (L == NULL)
        return L;
    if (strcmp(L->url, url) == 0) {
        List temp = L->next;
        free(L->url);
        free(L);
        return temp;
    }
    
    L->next = deleteLL(L->next, url);
    return L;
    
}

bool inLL(List L, char *url) {
    if (L == NULL)
        return false;
    if (strcmp(L->url, url) == 0)
        return true;
    
    return inLL(L->next, url);
}

//Won't need this

void showLL(List L) {
    if (L == NULL)
        putchar('\n');
    else {
        printf("%s ", L->url);
        showLL(L->next);
    }
}


int lengthLL(List L) {    
    if (L == NULL) {
        return 0;
    }
    return 1 + lengthLL(L->next);
}


void freeLL(List L) {
    if (L != NULL) {
        freeLL(L->next);
        free(L->url);
        free(L);
    }
}
