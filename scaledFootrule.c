//
//  scaledFootrule.c
//  COMP2521_assign2
//
//  Created by Ben Henderson on 24/10/18.
//  Copyright © 2018 Ben Henderson. All rights reserved.
//

#include "scaledFootrule.h"

static Set getC(Set C, FILE **rankings);
static double W(Url u, int p);
static int **hungarianAlgorithm(double **cost, int n);
static int getAssignmentArray(double **cost, int n, int **assignArray);
static int rankAssigned(int **assignArray, int j, int n);
static double findMin(double **cost, int n, int *mI, int *mJ);
static void freeNxN(void **a, int n);
static void printNxN(int **a, int n);

/*
int main(int argc, const char * argv[]) {
    // if no arguments are given display usage
    if (argc == 1) {
        printf("Usage: ./%s <rankA.txt> <rankB.txt>...", argv[0]);
        return 0;
    }
    // otherwise make an array of file pointers with each of the rankings
    int n = argc - 1;
    FILE **rankings = malloc(n * sizeof(FILE*));
    int i = 1;
    
    Set C = newSet(n);
    
    for (printf("Ranking files: "); i <= n; i++) {
        rankings[i-1] = fopen(argv[i], "r");
        if (rankings[i-1] == NULL) {
            printf("%s was not found\n", argv[i]);
        } else {
            printf("%s was succesfully found\n", argv[i]);
        }
    }
    
    C = getC(C, rankings);
    
    // now we should close the files and free the rankings array
    // TODO
    
    // now generate all the weights
    // malloc nxn array
    double **cost = malloc(C->size*sizeof(double*));
    for (int i = 0; i < C->size; i++) {
        cost[i] = malloc(C->size*sizeof(double));
        for (int j = 0; j < C->size; j++) {
            // now calculate W(C->array[i], j)
            cost[i][j] = W(C->array[i], j);
        }
    }
    
    // now start the hungarian algorithm, attempting to match urls in C
    // with a rank in P where the weight of these matches is minimum
    int **assignArray = hungarianAlgorithm(cost, C->size);
    printNxN(assignArray, n);
    
    // free cost since we don't need it anymore
    freeNxN((void **) cost, n);
    freeNxN((void **) assignArray, n);
    
}
*/
/* performs the hungarian algorithm on the n x n matrix cost
    https://en.wikipedia.org/wiki/Hungarian_algorithm
    Following the matrix interpretation from wikipedia
 
    For us the rows represent a different url, and the columns are the
    different possible placements of that url.
 
    So the cost matrix is essentially if we place the url, c (row) at position p (column)
    how different is this placement to the input rankings?
    i.e. Each element in cost is the scaled footrule distance W(c, p) where c is the row number
    and p is the column number
 
    Returns the assignArray, assignArray[i][j] = 1 if url i was assigned to pos j.
    The needs to free this assignArray
*/
int **hungarianAlgorithm(double **cost, int n) {
/*  ****** Step 1 *******
    Find the lowest value in each row and subtract every element in each row by this value
    This is essentially find the best placement of every url,
    After this every row will have at least one 0 element.
    If every column has a zero then we can just pick all the zeros and we've found
    an optimal solution!!
    Otherwise we have to progress to step 2
*/
    
    int goStep2 = 0;
    for(int i = 0; i < n; i++) {
        // for every row
        int min = cost[i][0];
        int zeros = 0; // count max amount of zeros in any column
        // for every column
        for(int j = 1; j < n; j++) {
            // find min element
            min = (cost[i][j] < min ? cost[i][j] : min);
        }
        // subtract every element
        for(int j = 0; j < n; j++) {
            // find min element
            cost[i][j] -= min;
            assert(cost[i][j] >= 0);
            if (cost[i][j] == 0) {
                zeros++;
            }
        }
        // if we have no zeros in any column we must go to step 2
        if (zeros == 0) {
            goStep2 = 1;
        }
    }
    int **assignArray = calloc(n, sizeof(int*));
    for (int i = 0; i < n; i++) {
        assignArray[n] = calloc(n, sizeof(int));
    }
    if (!goStep2 && getAssignmentArray(cost, n, assignArray)) {
        // we found an optimal ranking and can return
        printf("we found an optimal ranking and can return\n");
        return assignArray;
    }
    
/*  ****** Step 2 *******
    Subtract the minimum element of each column from that column.
    After this every column will have a 0, but there might not be a 1 to 1 assignment
    If not we have to go to step 3.
*/
    
    // for every column
    for(int j = 0; j < n; j++) {
        int min = cost[0][j];
        int zeros = 0; // count max amount of zeros in any column
        // for every row
        for(int i = 1; i < n; i++) {
            // find min element
            min = (cost[i][j] < min ? cost[i][j] : min);
        }
        // subtract every element
        for(int i = 0; i < n; i++) {
            // find min element
            cost[i][j] -= min;
            assert(cost[i][j] >= 0);
            if (cost[i][j] == 0) {
                zeros++;
            }
        }
    }
    // now we need to check if we can assign every url to a unique rank
    
    // while there is no optimal assignment repeat steps 3 & 4
    while (!getAssignmentArray(cost, n, assignArray)) {
    
        /*  ****** Step 3 *******
        Mark off the fewest number of columns and rows so that all the 0s are marked
        To do this assign as many tasks as possible.
        1. Mark all rows having no assignments
        2. Mark all (unmarked) columns having zeros in newly marked row(s)
        3. Mark all rows having assignments in newly marked columns
        4. Repeat for all non-assigned rows
         
        Now draw lines through all marked columns and unmarked rows.
        Essentially flip the marked rows
        */
        
        int mI[n]; // if the row corresponding to the index is marked
        int mJ[n]; // same but for columns - so mJ[0] > 0 -> first columns is marked
        
        for(int i = 0; i < n; i++) {
            mI[n] = 0;
            mJ[n] = 0;
        }
        
        // we have the assignArray from step 2
        // 1. mark all rows with no assignments
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                mI[i] += assignArray[i][j];
            }
            // if there has been no assignments in this row at this point then mI[i] = 0
            if (mI[i] == 0) {
                mI[i] = 1; // so mark this row
                // 2. if we've just marked a row then mark all (unmarked) columns which have zeros
                for (int j = 0; j < n; j++) {
                    if (mJ[j] == 0 && cost[i][j] == 0) {
                        // mark this column
                        mJ[j] = 1;
                        // 3. mark all rows having assignments in this newly marked column
                        for (int row = 0; row < n; row++) {
                            if (assignArray[row][j] == 1) {
                                mI[row] = 1;
                            }
                        }
                    }
                }
            } else {
                // otherwise there has been assignments so make it unmarked
                mI[i] = 0;
            }
        } // 4. at this point we repeat for the next row
        
        // now we flip the marked rows
        for (int i = 0; i < n; i++) {
            mI[i] = !mI[i];
        }
        
        /*  ****** Step 4 *******
        Now from the elements that are unmarked find the lowest value.
        Subtract this from every unmarked element and add it to every
        element covered by two lines (or marked in both rows and columns).
        */
        double min = findMin(cost, n, mI, mJ);
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                // if it's unmarked
                if (!mI[i] && !mJ[j]) {
                    cost[i][j] -= min;
                // else if it's marked in both row and cols
                } else if (mI[i] && mJ[j]) {
                    cost[i][j] += min;
                }
            }
        }
        
    } // now repeat 3 and 4 until an assignment is possible (back to while loop above)
    
    // once we've broken out of the while we found an optimal ranking and can return
    printf("we found an optimal ranking in step 2 and can return\n");
    
    return assignArray;
}
// returns 1 if optimally assigned, 0 if not
// also edits assignArray, so that assignArray[i][j] is 1 at i, j if i, j was assigned
int getAssignmentArray(double **cost, int n, int **assignArray) {
    int optimallyAssined = 1;
    // initilise the assignment array
    for(int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            assignArray[i][j] = 0;
        }
    }
    // for every url
    for(int i = 0; i < n; i++) {
        int j = 0;
        // find the first 0 cost in this row where the corresping rank of that cost has not yet been assigned
        while ((cost[i][j] != 0 || rankAssigned(assignArray, j, n)) && j != n) {
            j++;
        }
        // if the cost is zero and this rank has not already been assigned
        if (cost[i][j] == 0 && !rankAssigned(assignArray, j, n)) {
            // if we found an assignment set it in the array
            assignArray[i][j] = 1;
        } else if (j == n) {
            optimallyAssined = 0;
        }
    }
    return optimallyAssined;
}

// from the assignment array returns if the rank j has been assigned
int rankAssigned(int **assignArray, int j, int n) {
    int isAssigned = 0;
    for(int i = 0; i < n; i++) {
        // isAssigned will be false if every entry is false
        // otherwise isAssigned > 0 - i.e. true
        isAssigned += assignArray[i][j];
    }
    return isAssigned;
}

// looks for the minimum value in cost out of the elements that are unmarked
double findMin(double **cost, int n, int *mI, int *mJ) {
    double min = -1;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            // if it's unmarked
            if (!mI[i] && !mJ[j]) {
                // just for the first time
                if (min == -1) {
                    min = cost[i][j];
                } else {
                    // general case
                    min = (cost[i][j] < min ? cost[i][j] : min);
                }
            }
        }
    }
    return min;
}


// calculates scaled-footrule distance
double W(Url u, int j) {
    Set C = u->p;
    double w = 0;
    
    // sum for every input rank
    for (int i = 0; i < C->nRanks; i++) {
        w += abs(u->ranks[i]/C->sizeInputRanks[i] - j/C->size);
    }
    return w;
}

void freeNxN(void **a, int n) {
    for (int i = 0; i < n; i++) {
        free(a[i]);
    }
    free(a);
}

void printNxN(int **a, int n) {
    if (n == 0) return;
    for (int i = 0; i < n; i++) {
        int j = 0;
        for (; j < n - 1; j++) {
            printf("%d\t", a[i][j]);
        }
        printf("%d\n", a[i][j]);
    }
}

// fills in the set from an array of file pointers to the input rankings
Set getC(Set C, FILE **rankings) {
    // init the cardinality count
    C->sizeInputRanks = calloc(C->nRanks, sizeof(int));
    
    // loop through the rankings array
    for (int i = 0; i < C->nRanks; i++) {
        // reads the file line by line
        char line[1000] = "\0";
        
        // the line count in the file - rank for this url (0 indexed)
        int rankCount = 0;
        
        while (fgets(line, 1000, rankings[i]) != NULL) {
            // see if we have this url in the set already
            Url u = inSet(C, line);
            if (u == NULL) {
                // if we haven't got this url in C yet add it
                addToSet(C, line, rankCount);
            } else {
                // if it is in the set then add this rank
                u->ranks[i] = rankCount;
            }
            rankCount++;
        }
        // once we've got through an entire ranking file set the size of this ranking file
        C->sizeInputRanks[i] = rankCount;
        
    }
    // once we know the size of C
    // create an array of urls for easy indexing
    C->array = malloc(C->size * sizeof(Url));
    Url u = C->head;
    int i = 0;
    while (u != NULL) {
        C->array[i++] = u;
        u = u->next;
    }
    return C;
}


// ***** Methods for the dodgy set ******

Set newSet(int n) {
    Set s = malloc(sizeof(Set));
    s->head = NULL;
    s->size = 0;
    s->nRanks = n;
    s->sizeInputRanks = NULL;
    return s;
}

Set addToSet(Set C, char* u, int rank) {
    // hold the current head temporarily
    Url h = C->head;
    // add the new url to the beginning of the linked list
    C->head = malloc(sizeof(url));
    // init all the fields
    C->head->name = malloc(strlen(u)*sizeof(char*));
    strcpy(C->head->name, u);
    C->head->ranks = calloc(C->nRanks, sizeof(int));
    C->head->ranks[0] = rank;
    C->head->next = h;
    C->head->p = C;
    // increase the size by 1
    C->size++;
    return C;
}

Url inSet(Set C, char* u) {
    Url i = C->head;
    while (i != NULL) {
        if (strcmp(i->name, u) == 0) {
            return i;
        } else {
            i = i->next;
        }
    }
    return NULL;
}
