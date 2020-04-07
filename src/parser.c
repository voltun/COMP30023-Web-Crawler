#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HTTP_STRLEN 7

/*
Finds all valid html redirects in the given text
Returns a 2D char array of found url
*/
// char** parse_html(char* text)
// {
//     return ;
// }

/*
Removes http:// from given text
*/
int rem_http(char* text)
{
    strcpy(text, text + HTTP_STRLEN);
    return 0;
}

char* substring(char* text, int offset, int length)
{
    char* substr = NULL;
    int n;

    substr = malloc((length+1)*sizeof(char));
    if (!substr)
    {
        printf("\nmalloc() failed!\n");
        exit(EXIT_FAILURE);
    }

    for (n = 0; n < length; n++)
    {
        *(substr + n) = *(text + offset - 1);
        text++;
    }

    *(substr + n) = '\0';

    return substr;
}