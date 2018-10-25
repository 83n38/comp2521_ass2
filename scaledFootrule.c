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
int getNextRow(double **cost, int **assignArray, int *checked, int n);
static double findMin(double **cost, int n, int *mI, int *mJ);
static void freeNxN(void **a, int n);
//static void printNxN(double **a, int n);
//void freeSet(Set s);
//void intPrintNxN(int **a, int n);
static void trim(char *str);


int main(int argc, const char * argv[]) {
    // if no arguments are given display usage
    if (argc == 1) {
        printf("Usage: ./%s <rankA.txt> <rankB.txt>...", argv[0]);
        return 0;
    }
    // otherwise make an array of file pointers with each of the rankings
    int n = argc - 1;
    FILE **rankings = malloc(n * sizeof(FILE*));
    
    Set C = newSet(n);
    //printf("Ranking files: ");
    for (int i = 1; i <= n; i++) {
        rankings[i-1] = fopen(argv[i], "r");
        if (rankings[i-1] == NULL) {
            printf("%s was not found\n", argv[i]);
            return 0;
        } else {
            //printf("%s was succesfully found\n", argv[i]);
        }
    }
    
    C = getC(C, rankings);
    
    // now we should close the files and free the rankings array
    for (int i = 0; i < n; i++) {
        fclose(rankings[i]);
    }
    free(rankings);
    
    // now generate all the weights
    // malloc nxn array
    n = C->size;
    double **cost = malloc(n*sizeof(double*));
    double **costClean = malloc(n*sizeof(double*));
    // a testing example
    //double ex1[10][10] = {{0,  0,  0,  0,  55,  96,  81,  30,  72,  82}, {79,  69,  8 ,  48,  83,  49,  33,  67,  12,  90}, {6 ,  85,  96,  22,  51,  43,  72,  53,  17,  11}, {4 ,  29,  53,  15,  83,  11,  3 ,  95,  6 ,  49}, {50,  11,  9 ,  29,  50,  4 ,  71,  49,  42,  61}, {26,  58,  83,  47,  62,  80,  35,  87,  89,  73}, {30,  51,  99,  93,  92,  91,  56,  31,  11,  50}, {95,  55,  52,  74,  59,  20,  44,  48,  44,  84}, {66,  73,  65,  21,  3 ,  1 ,  99,  17,  82,  60}, {39,  73,  58,  5 ,  65,  50,  46,  84,  23,  2}};
    for (int i = 0; i < C->size; i++) {
        cost[i] = malloc(C->size*sizeof(double));
        costClean[i] = malloc(C->size*sizeof(double));
        assert(cost[i] != NULL && costClean[i] != NULL);
        for (int j = 0; j < C->size; j++) {
            // now calculate W(C->array[i], j)
            cost[i][j] = W(C->array[i], j);
            costClean[i][j] = cost[i][j];
        }
    }
    // now start the hungarian algorithm, attempting to match urls in C
    // with a rank in P where the weight of these matches is minimum
    int **assignArray = hungarianAlgorithm(cost, C->size);

    // summ up the cost
    double totalCost = 0;
    for (int j = 0; j < n; j++) {
        for (int i = 0; i < n; i++) {
            if (assignArray[i][j]) {
                totalCost += costClean[i][j];
            }
        }
    }
    printf("%.6f\n", totalCost);
    for (int j = 0; j < n; j++) {
        for (int i = 0; i < n; i++) {
            if (assignArray[i][j]) {
                printf("%s\n", C->array[i]->name);
            }
        }
    }
    
    // free cost since we don't need it anymore
    freeNxN((void **) cost, n);
    freeNxN((void **) assignArray, n);
    
    //freeSet(C);
    
}

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
        double min = cost[i][0];
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
            //assert(cost[i][j] >= 0);
            if (fabs(cost[i][j]-0) < EPSILON) {
                zeros++;
            }
        }
        // if we have no zeros in any column we must go to step 2
        if (zeros == 0) {
            goStep2 = 1;
        }
    }
    int **assignArray = malloc(n*sizeof(int*));
    assert(assignArray != NULL);

    for (int i = 0; i < n; i++) {
        assignArray[i] = malloc(n*sizeof(int));
        assert(assignArray[i] != NULL);
        for (int j = 0; j < n; j++) {
            assignArray[i][j] = 0;
        }
    }
    
    if (!goStep2 && getAssignmentArray(cost, n, assignArray)) {
        // we found an optimal ranking and can return
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
            if (fabs(cost[i][j]-0) < EPSILON) {
                zeros++;
            }
        }
    }
    
    // now we need to check if we can assign every url to a unique rank
    
    // while there is no optimal assignment repeat steps 3 & 4
    int repeat = 0;
    while (!getAssignmentArray(cost, n, assignArray)) {
        repeat++;
        /*  ****** Step 3 *******
        Mark off the fewest number of columns and rows so that all the 0s are marked
        To do this assign as many tasks as possible.
        1. Mark all rows having no assignments
        2. If a row is marked and has a 0 mark the correspding column
        3. If a column is marked and has an assignment, mark the corresponding row
        4. Repeat for steps 2-3 until no more can be marked
         
        Now draw lines through all marked columns and unmarked rows.
        Essentially flip the marked rows
        */
        
        int mI[n]; // if the row corresponding to the index is marked
        int mJ[n]; // same but for columns - so mJ[0] > 0 -> first columns is marked
        
        for(int i = 0; i < n; i++) {
            mI[i] = 0;
            mJ[i] = 0;
        }
        
        // we have the assignArray from step 2
        int marks = 0; // count the marks so we know when to stop
        // 1. mark all rows with no assignments
        for (int i = 0; i < n; i++) {
            int sumAssignments = 0;
            for (int j = 0; j < n; j++) {
                sumAssignments += assignArray[i][j];
            }
            // if there has been no assignments in this row then mark it
            if (sumAssignments == 0) {
                mI[i] = 1; // so mark this row
                marks++;
            }
        }
        while (marks > 0) {
            marks = 0;
            // now go through all the marked rows
            for (int i = 0; i < n; i++) {
                // if the row is currently unmarked then skip it
                if (mI[i] == 0) continue;
                // 2. and mark all (unmarked) columns which have zeros
                for (int j = 0; j < n; j++) {
                    if (mJ[j] == 0 && fabs(cost[i][j]-0) < EPSILON) {
                        // mark this column
                        mJ[j] = 1;
                        marks++;
                        // 3. mark all unmarked rows having assignments in this newly marked column
                        for (int row = 0; row < n; row++) {
                            if (assignArray[row][j] == 1 && mI[row] == 0) {
                                mI[row] = 1;
                                marks++;
                            }
                        }
                    }
                }
            }
        }
        
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
    return assignArray;
}
// returns 1 if optimally assigned, 0 if not
// also edits assignArray, so that assignArray[i][j] is 1 at i, j if i, j was assigned
int getAssignmentArray(double **cost, int n, int **assignArray) {
    int optimallyAssigned = 1;
    // initilise the assignment array
    for(int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            assignArray[i][j] = 0;
        }
    }

    // for every url
    int checked[n];
    for (int i = 0; i < n; i++) {
        checked[i] = 0;
    }
    int done = 0;
    
    while(!done) {
        // get the row with the least available options to select first
        int i = getNextRow(cost, assignArray, checked, n);
        
        // selects j with the least available options too
        // go through all the 0s in this row and evaluate which should be chosen based on how many
        // other zeros are in that same column
        int minZeros = n;
        int minIndex = n;
        for (int j = 0; j < n; j++) {
            // if the cost is non zero or this rank is already assigned ignore it
            if (fabs(cost[i][j]-0) > EPSILON || rankAssigned(assignArray, j, n)) continue;
            // find how many zeros are in this column
            int zeros = 0;
            for (int row = 0; row < n; row++) {
                if(fabs(cost[row][j]-0) < EPSILON) {
                    zeros++;
                }
            }
            if (zeros < minZeros) {
                minZeros = zeros;
                minIndex = j;
            }
        }
        // we should select the i, j element
        int j = minIndex;
        // if the cost is zero and this rank has not already been assigned
        if (fabs(cost[i][j]-0) < EPSILON && !rankAssigned(assignArray, j, n)) {
            // if we found an assignment set it in the array
            assignArray[i][j] = 1;
        } else if (j == n) {
            // if we we're able to find any options for a given row then assignment is impossible
            optimallyAssigned = 0;
        }
        
        done = 1;
        for (int i = 0; i < n; i++) {
            done = done && checked[i];
        }
    }
    return optimallyAssigned;
}

// this returns the next row to check assignment
// it will return rows based on the number of zeros in that row, lowest to highest
int getNextRow(double **cost, int **assignArray, int *checked, int n) {
    // go through every row that isnt checked
    int minIndex = n;
    int minZeros = n;
    for (int i = 0; i < n; i++) {
        if (checked[i]) continue;
        int count = 0;
        // see how many zeros it has
        // but only count the zero if we can choose it, i.e. it's not already assigned
        for (int j = 0; j < n; j++) {
            if (fabs(cost[i][j]-0) < EPSILON && !rankAssigned(assignArray, j, n)) {
                count++;
            }
        }
        if (count <= minZeros) {
            minZeros = count;
            minIndex = i;
        }
        
    }
    // say that we've checked this row
    checked[minIndex] = 1;
    return minIndex;
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
// doesn't work if something is not in both lists
double W(Url u, int j) {
    Set C = u->p;
    double w = 0;
    
    // sum for every input rank
    for (int i = 0; i < u->ranksSize; i++) {
        w += fabs((double)(u->ranks[i] + 1)/(double)(C->sizeInputRanks[i]) - (double)(j+1)/(double)C->size);
    }
    return w;
}

void freeNxN(void **a, int n) {
    for (int i = 0; i < n; i++) {
        free(a[i]);
    }
    free(a);
}

// fills in the set from an array of file pointers to the input rankings
Set getC(Set C, FILE **rankings) {
    // init the cardinality count
    C->sizeInputRanks = calloc(C->nRanks + 1, sizeof(int));
    
    // loop through the rankings array
    for (int i = 0; i < C->nRanks; i++) {
        // reads the file line by line
        char line[1000] = "\0";
        
        // the line count in the file - rank for this url (0 indexed)
        int rankCount = 0;
        
        while (fgets(line, 1000, rankings[i]) != NULL) {
            // trim the url
            trim(line);
            // see if we have this url in the set already
            Url u = inSet(C, line);
            if (u == NULL) {
                // if we haven't got this url in C yet add it
                addToSet(C, line, rankCount);
            } else {
                // if it is in the set then add this rank
                u->ranks[i] = rankCount;
                // increase the size
                u->ranksSize++;
            }
            rankCount++;
        }
        // once we've got through an entire ranking file set the size of this ranking file
        C->sizeInputRanks[i] = rankCount;
        
    }
    // once we know the size of C
    // create an array of urls for easy indexing
    C->array = malloc((C->size +1) * sizeof(Url));
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
    Set s = malloc(sizeof(Set) + 1);
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
    C->head->name = malloc((strlen(u) + 1)*sizeof(char*));
    strcpy(C->head->name, u);
    C->head->ranks = calloc(C->nRanks + 1, sizeof(int));
    C->head->ranksSize = 1;
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

/*void freeSet(Set s) {
    // free all the urls
    for (int i = 0; i < s->size; i++) {
        free(s->array[i]->name);
        free(s->array[i]->ranks);
        free(s->array[i]);
    }
    // free the arrays
    free(s->array);
    free(s->sizeInputRanks);
    free(s);
}*/

/* remove leading/trailing spaces from a string, as well as
 '.' (dot), ',' (comma), ';' (semicolon), ? (question mark)
 characters if at end of word.
 P.S. Code is modifed from starting code in COMP1521 Ass02
 */
static void trim(char *str) {
    int first, last;
    first = 0;
    while (isspace(str[first])) first++;
    
    last  = strlen(str)-1;
    while (isspace(str[last]) || str[last] == '.' || str[last] == '?' ||
           str[last] == ';' || str[last] == ',' || str[last] == '\n') {
        last--;
    }
    int i, j = 0;
    for (i = first; i <= last; i++) {
        str[i] = tolower(str[i]);
        str[j++] = str[i];
    }
    str[j] = '\0';
}

