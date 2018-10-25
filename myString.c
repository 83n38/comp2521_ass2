
#include "myString.h"

/* New implementaions of functions in string.h which
   are not compatable with gcc -Wall -lm -std=c11 */


/* New strdup. From stack overflow */
//https://stackoverflow.com/questions/37132549/implementation-of-strdup-in-c-programming
char *mystrdup2(char *src) {
    char *str;
    char *p;
    int len = 0;
    
    while (src[len])
        len++;
    
    str = calloc(len + 10, sizeof(char));
    p = str;
    while (*src)
        *p++ = *src++;
    *p = '\0';
    return str;
}

/* New strsep. From this link */
//https://code.woboq.org/userspace/glibc/string/strsep.c.html
char *mystrsep (char **stringp, const char *delim) {
    char *begin, *end;
    begin = *stringp;
    if (begin == NULL)
        return NULL;
    /* Find the end of the token.  */
    end = begin + strcspn(begin, delim);
    if (*end)
    {
        /* Terminate the token and set *STRINGP past NUL character.  */
        *end++ = '\0';
        *stringp = end;
    }
    else
    /* No more delimiters; this is the last token.  */
        *stringp = NULL;
    return begin;
}
