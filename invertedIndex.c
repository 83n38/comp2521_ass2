// Binary Search IList ADT implementation ... COMP2521

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <math.h>
#include "invertedIndex.h"
#include "list.h"

#define RANDOM_ROOT_INSERT (random() % 10 < 4)   // 40% chance

#define word(IList)  ((IList)->word)
#define left(IList)  ((IList)->left)
#define right(IList)  ((IList)->right)

// make a new node containing word
IList newNode(Word word) {
    IList new = malloc(sizeof(Node));
    assert(new != NULL);
    word(new) = mystrdup2(word);
    new->urlList = NULL;
    //new->idf = malloc(sizeof(double));
    left(new) = right(new) = NULL;
    return new;
}

// create a new empty IList
IList newIList() {
    return NULL;
}

// free memory associated with IList
void freeIList(IList t) {
    if (t != NULL) {
        free(t->word);
        freeIList(left(t));
        freeIList(right(t));
        free(t);
    }
}

//Don't really need to show IList

// display IList horizontally
// code by Jin Qu
/*
void showIListR(IList t, int index, char **record, char *largest) {
    // record largest element along the way
    if (t != NULL) {
        record[index] = word(t);
        if (strcmp(word(t), largest) > 0) {
            strcpy(largest, word(t));
        }
        showIListR(left(t), 2 * index + 1, record, largest);
        showIListR(right(t), 2 * index + 2, record, largest);
    }
}

void showIList(IList t) {
    int i, h = IListHeight(t);
    char largest[100] = "a";
    
    //use an array of pointers to distinguish NULL node and node with value 0
    char **record = calloc(pow(2, h + 1) - 1, sizeof(char*));
    assert(record != NULL);
    showIListR(t, 0, record, largest);
    int size, lv = 0;
    size = 1;
    for (i = 0; i < pow(2, h + 1) - 1; i++) {
        int space = size * ((int)(pow(2, h - lv + 1) - 1) / 2);
        printf("%*s", space, "");
        
        // centralize nodes
        if (record[i]) {
            printf("%*c", size, *record[i]);
        } else {
            printf("%*s", size, "");
        }
        printf("%*s", space + size, "");
        if (i == pow(2, lv + 1) - 2) {
            printf("\n\n");
            lv++;
        }
    }
    free(record);
}
*/
 
// compute height of IList
int IListHeight(IList t) {
    if (t == NULL) {
        return 0;
    } else {
        int lheight = 1 + IListHeight(left(t));
        int rheight = 1 + IListHeight(right(t));
        if (lheight > rheight)
            return lheight;
        else
            return rheight;
    }
}

// count #nodes in IList
int IListNumNodes(IList t) {
    if (t == NULL)
        return 0;
    else
        return 1 + IListNumNodes(left(t)) + IListNumNodes(right(t));
}

// check whether a key is in a IList
// return NULL if false
// return Node if True
IList IListSearch(IList t, Word word) {
    if (t == NULL)
        return NULL;
    else if (strcmp(word, word(t)) < 0)
        return IListSearch(left(t), word);
    else if (strcmp(word, word(t)) > 0)
        return IListSearch(right(t), word);
    else                                 // word == word(t)
        return t;
}

// insert a new item into a IList
IList IListInsert(IList t, Word word) {
    if (t == NULL)
        t = newNode(word);
    else if (strcmp(word, word(t)) < 0)
        left(t) = IListInsert(left(t), word);
    else if (strcmp(word, word(t)) > 0)
        right(t) = IListInsert(right(t), word);
    return t;
}

IList joinILists(IList t1, IList t2) {
    if (t1 == NULL)
        return t1;
    else if (t2 == NULL)
        return t2;
    else {
        IList curr = t2;
        IList parent = NULL;
        while (left(curr) != NULL) {    // find min element in t2
            parent = curr;
            curr = left(curr);
        }
        if (parent != NULL) {
            left(parent) = right(curr);  // unlink min element from parent
            right(curr) = t2;
        }
        left(curr) = t1;
        return curr;                    // min element is new root
    }
}

// delete an item from a IList
IList IListDelete(IList t, Word word) {
    if (t != NULL) {
        if (strcmp(word, word(t)) < 0)
            left(t) = IListDelete(left(t), word);
        else if (strcmp(word, word(t)) > 0)
            right(t) = IListDelete(right(t), word);
        else {
            IList new;
            if (left(t) == NULL && right(t) == NULL)
                new = NULL;
            else if (left(t) == NULL)    // if only right subIList, make it the new root
                new = right(t);
            else if (right(t) == NULL)   // if only left subIList, make it the new root
                new = left(t);
            else                         // left(t) != NULL and right(t) != NULL
                new = joinILists(left(t), right(t));
            free(t);
            t = new;
        }
    }
    return t;
}

IList rotateRight(IList n1) {
    if (n1 == NULL || left(n1) == NULL)
        return n1;
    IList n2 = left(n1);
    left(n1) = right(n2);
    right(n2) = n1;
    return n2;
}

IList rotateLeft(IList n2) {
    if (n2 == NULL || right(n2) == NULL)
        return n2;
    IList n1 = right(n2);
    right(n2) = left(n1);
    left(n1) = n2;
    return n1;
}

IList insertAtRoot(IList t, Word word) {
    if (t == NULL) {
        t = newNode(word);
    } else if (strcmp(word, word(t)) < 0) {
        left(t) = insertAtRoot(left(t), word);
        t = rotateRight(t);
    } else if (strcmp(word, word(t)) > 0) {
        right(t) = insertAtRoot(right(t), word);
        t = rotateLeft(t);
    }
    return t;
}

IList insertRandom(IList t, Word word) {
    if (t == NULL)
        t = newNode(word);
    if (RANDOM_ROOT_INSERT)
        return insertAtRoot(t, word);
    else
        return IListInsert(t, word);
}

IList insertAVL(IList t, Word word) {
    if (t == NULL)
        return newNode(word);
    if (strcmp(word, word(t)) == 0)
        return t;
    
    if (strcmp(word, word(t)) < 0)
        left(t) = insertAVL(left(t), word);
    else
        right(t) = insertAVL(right(t), word);
    
    int hL = IListHeight(left(t));
    int hR = IListHeight(right(t));
    if ((hL - hR) > 1) {
        if (strcmp(word, word(t)) > 0)
            left(t) = rotateLeft(left(t));
        t = rotateRight(t);
    } else if ((hR - hL) > 1) {
        if (strcmp(word, word(t)) < 0)
            right(t) = rotateRight(right(t));
        t = rotateLeft(t);
    }
    
    return t;
}

IList partition(IList t, int i) {
    if (t != NULL) {
        assert(0 <= i && i < IListNumNodes(t));
        int m = IListNumNodes(left(t));
        if (i < m) {
            left(t) = partition(left(t), i);
            t = rotateRight(t);
        } else if (i > m) {
            right(t) = partition(right(t), i-m-1);
            t = rotateLeft(t);
        }
    }
    return t;
}

IList rebalance(IList t) {
    int n = IListNumNodes(t);
    if (n >= 3) {
        t = partition(t, n/2);           // put node with median key at root
        left(t) = rebalance(left(t));    // then rebalance each subIList
        right(t) = rebalance(right(t));
    }
    return t;
}

void outputIL(IList t, FILE *fp) {
    
    if (t == NULL) {
        return;
    }
    
    outputIL(left(t), fp);
    
    fprintf(fp, "%s  ", t->word);
    for (List curr = t->urlList; curr != NULL; curr = curr->next) {
        fprintf(fp, "%s ", curr->url);
    }
    fprintf(fp, "\n");
    
    outputIL(right(t), fp);
}

/*calculates the idf for entire inverted List*/

IList calculate_idf(IList t, int nPages) {
    
    if(t == NULL) {
        return NULL;
    }
    
    left(t) = calculate_idf(left(t), nPages);
    
    printf("Before: %s\n", t->word);    
    t->idf = log10f( (float)nPages / (float)lengthLL(t->urlList) );
    printf("After: %s\n", t->word);
    
    right(t) = calculate_idf(right(t), nPages);
    
    return t;
}

