
#include "myString.h"

/* New strdup. Modified from stack overflow */
char *mystrdup2(char *src) {
    char *str;
    char *p;
    int len = 0;
    
    while (src[len])
        len++;
    
    str = calloc(len + 1, sizeof(char));
    p = str;
    while (*src)
        *p++ = *src++;
    *p = '\0';
    return str;
}
