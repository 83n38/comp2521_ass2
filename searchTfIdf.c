//
//  searchTfIdf.c
//  Ass2
//
//  Created by Riley Vozzo on 23/10/18.
//  Copyright © 2018 Riley Vozzo. All rights reserved.
//

#include "searchTfIdf.h"

int main(int argc, char * argv[]) {
    
    List L = getCollection();
    IList IL = getInvertedList(L);
    
    if (argc == 1) {
        printf("Usage: ./%s <search term> <search term>...", argv[0]);
        return 0;
    }
    int nTerms = argc - 1;
    char **searchTerms = malloc(nTerms * sizeof(char*));
    for (int i = 1; i <= nTerms; i++) {        
        searchTerms[i-1] = mystrdup2(argv[i]);
    }
    //Get matching urlList with tf_idf values
    List tf_idf_list = get_tf_idf_list(IL, L, searchTerms, nTerms);
 
    // init function pointer
    int (*comparePtr)(Node *, Node *);
    // sort by tf-idf
    comparePtr = &compareTf_idf;
    bubbleSort(tf_idf_list, comparePtr);
    
    for (List curr = tf_idf_list; curr != NULL; curr = curr->next) {
        printf("%s %0.6f\n", curr->url, curr->tf_idf);
    }
    
    freeLL(L);
    freeLL(tf_idf_list);
    freeIList(IL);
    for (int i = 0; i < nTerms; i++) {
        free(searchTerms[i]);
    }
    free(searchTerms);
    
    return 0;
}



/* Returns an LL of urls which match with the search terms,
   along with their tf_idf */
List get_tf_idf_list(IList IL, List collectionL, char **searchTerms, int nTerms) {
    
    List tf_idf_list = NULL;
    
    for (int i = 0; i < nTerms; i++) {
        
        BSTNode *wordNode = IListSearch(IL, searchTerms[i]);
        
        Node *curr = wordNode->urlList;
        while (curr != NULL) {
            
            Node *tf_idf_list_node = inLL(tf_idf_list, curr->url);
            //if url not in LL, add and initialise it
            if(tf_idf_list_node == NULL) {
                tf_idf_list = insertLL(tf_idf_list, curr->url);
                tf_idf_list_node = inLL(tf_idf_list, curr->url);
                Node *collNode = inLL(collectionL, curr->url);
                tf_idf_list_node->nWords = collNode->nWords;
                tf_idf_list_node->tf_idf = 0;
                tf_idf_list_node->matchCount = 0;
            }
            
            tf_idf_list_node->tf_idf += wordNode->idf *
                ((float)curr->matchCount/(float)tf_idf_list_node->nWords);
            
            tf_idf_list_node->matchCount++;
            
            curr = curr->next;
        }
    }
    
    return tf_idf_list;
}
