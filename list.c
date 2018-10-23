// Linked list implementation ... COMP2521
#include "list.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static void swap(Node *a, Node *b);

Node *makeNode(char *url) {
    Node *new = malloc(sizeof(Node));
    assert(new != NULL);
    new->url = mystrdup2(url);
    new->next = NULL;
    return new;
}

List insertLL(List L, char *url) {
    if (inLL(L, url))
        return L;
    
    // add new node at the beginning
    Node *new = makeNode(url);
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

List inLL(List L, char *url) {
    if (L == NULL)
        return NULL;
    if (strcmp(L->url, url) == 0)
        return L;
    
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

void bubbleSort(List head, int (*compare)(Node*, Node*)) {
    int swapped = 1;
    Node *curr;
    Node *end = NULL;
    
    /* Checking for empty list */
    if (head == NULL) {
        fprintf(stderr, "pageRankList is empty");
        return;
    }
    while (swapped) {
        swapped = 0;
        curr = head;
        while (curr->next != end)
        {
            if ((*compare)(curr, curr->next) < 0)
            {
                swap(curr, curr->next);
                swapped = 1;
            }
            curr = curr->next;
        }
        end = curr;
    }
}

/* function to swap data of two pageRankList nodes a and b*/
static void swap(Node *a, Node *b) {
    char *tempUrl = a->url;
    int tempNout = a->nOut;
    double tempRank = a->rank;
    int tempMatchCount = a->matchCount;
    a->url = b->url;
    a->nOut = b->nOut;
    a->rank = b->rank;
    a->matchCount = b->matchCount;
    
    b->url = tempUrl;
    b->nOut = tempNout;
    b->rank = tempRank;
    b->matchCount = tempMatchCount;
}


// Different sort options
// each is a comparison function

//if a < b returns negative
//if a == b returns zero
//if a > b returns positive

// by rank
int compareRanks(Node *a, Node *b) {
    if(a->rank == b->rank) {
        return 0;
    } if (a->rank > b->rank) {
        return 1;
    }
    // a < b
    return -1;
}

// by matches
int compareMatches(Node *a, Node *b) {
    return a->matchCount - b->matchCount;
}

