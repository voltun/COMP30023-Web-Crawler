#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#define HTTP_STRLEN 7
#define ANCHOR_START "<a href=\""

int get_full_url(char* url, char* text);

/*
Finds all valid html redirects in the given text
Returns a 2D char array of found url
*/
char** parse_html(char* text, int max_url_num, int max_url_len)
{
    char** url_list = NULL;
    char* body = NULL;
    int count = 0;

    url_list = (char **)malloc(max_url_num*sizeof(char *));
    if (!url_list)
    {
        printf("\nmalloc() failed!\n");
        exit(EXIT_FAILURE);
    }

    for (int i=0; i < max_url_num; i++)
    {
        url_list[i] = (char *)malloc((max_url_len+1)*sizeof(char));
        if(!url_list[i])
        {
            printf("\nmalloc() failed!\n");
            exit(EXIT_FAILURE);
        }
        bzero(url_list[i], max_url_len);
    }

    //Remove response header
    body = strstr(text, "\r\n\r\n");
    body = body + 4;

    //Find URL
    while ((body = strstr(body, ANCHOR_START)) != NULL)
    {
        body = body + strlen(ANCHOR_START);
        get_full_url(url_list[count], body);
        count += 1;
    }

    return url_list;
}

/*
Removes http:// from given text
*/
int rem_http(char* text)
{
    strcpy(text, text + HTTP_STRLEN);
    return 0;
}

/*
Splits the url into hostname and its remaining string
Takes a truncated url with no http://
Stores the remaining string in the input text char*
Returns char*, the split hostname
*/
char* split_hostname(char* url)
{
    int count = 0;
    char* hostname = NULL;

    hostname = malloc((strlen(url) + 1)*sizeof(char));

    while ((count < (int)strlen(url)) && url[count] != '/')
    {
        hostname[count] = url[count];
        count++;
    }

    hostname[count] = '\0';
    
    if ((strlen(url) - strlen(hostname) > 0))
    {
        strcpy(url, url + count);
    }
    else
    {
        url[0] = '/';
        url[1] = '\0';
    }
    
    return hostname;
}

/*
Gets the first full url from a given string
*/
int get_full_url(char* url, char* text)
{
    char c;
    int n = 0;

    while ((c = text[n]) != '\"')
    {
        url[n] = c;
        n += 1;
    }

    url[n] = '\0';
    return 0;
}
