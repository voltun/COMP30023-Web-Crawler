#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/types.h>
#include <regex.h>
#include <ctype.h>

#define HTTP_STRLEN 7
#define MAX_URL_LEN 1000
#define ANCHOR_START "href=\""
#define REGEX_HTTP "http://"
#define NON_VALID_URL_REGEX "([.]|[..])/|#|%"

int get_full_url(char* url, char* hostname, char* text);
int parse_valid_url(char**);
int rem_trail_slash(char*);
int rem_precede_slash(char*);
int htm_to_html(char*);
void rem_whitespace(char*);

/*
Finds all valid html redirects in the given text
Returns a 2D char array of found url
*/
char** parse_html(char* text, char* ori_hostname, int max_url_num, int max_url_len)
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

    //Remove all whitespace in message body
    rem_whitespace(body);

    //COnvert all characters in message to lowercase
    for (int i=0; body[i] != '\0'; i++)
    {
        if (isalpha(body[i]))
        {
            body[i] = tolower(body[i]);
        }
    }

    //Find URL by matching to "href="
    while ((body = strstr(body, ANCHOR_START)) != NULL)
    {
        body = body + strlen(ANCHOR_START);
        //Regenerate URL into absolute URL if needed
        get_full_url(url_list[count], ori_hostname, body);
        count += 1;
    }

    //Determine URL to be crawled to or not 
    parse_valid_url(url_list);

    free(ori_hostname);
    free(text);

    return url_list;
}

/*
Removes http:// from given text
*/
int rem_http(char* text)
{
    regex_t regex;
    char* a = NULL;
    int check, status;

    check = regcomp(&regex, REGEX_HTTP, REG_EXTENDED);
    if (check)
    {
        printf("Regex failed!\n");
        exit(EXIT_FAILURE);
    }
    status = regexec(&regex, text, 0, NULL, 0);
    
    //http:// substring not found
    if(!status)
    {
        a = malloc(strlen(text)*sizeof(char));
        bzero(a, strlen(text));
        //Append "http://" at the start of url
        strcpy(a, text+HTTP_STRLEN);
        strcpy(text, a);
    }

    free(a);
    regfree(&regex);

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

    //Stores the first substring until a '/' is found 
    while ((count < (int)strlen(url)) && url[count] != '/')
    {
        hostname[count] = url[count];
        count++;
    }

    hostname[count] = '\0';
    
    //Check if url has any uri links, or point to root
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
 *Removes all whitespace characters from a given string
 */
void rem_whitespace(char* text)
{
    int count = 0;

    for (int i=0; text[i] != '\0'; i++)
    {
        if (text[i] != ' ')
        {
            text[count++] = text[i];
        }
    }
    text[count] = '\0';
}

/*
Gets the first matched url link from a given string
If URL found is not absolute, will regenerate
*/
int get_full_url(char* url, char* hostname, char* text)
{
    char* hostcopy;
    regex_t regex;
    int check, status;
    char c;
    int n = 0;

    while ((c = text[n]) != '\"')
    {
        url[n] = c;
        n += 1;
    }

    url[n] = '\0';
    
    //Finds http:// in the url, no means relative url
    check = regcomp(&regex, REGEX_HTTP, REG_EXTENDED);
    if (check)
    {
        printf("Bad regex!\n");
        exit(EXIT_FAILURE);
    }
    status = regexec(&regex, url, 0, NULL, 0);
    if (!status)
    {
        return 0;
    }
    else if (status == REG_NOMATCH)
    {
        //Check implied protocol
        if (url[0] == '/' && url[1] == '/')
        {
            hostcopy = malloc((strlen("http:")+strlen(url)+1)*sizeof(char));
            if (!hostcopy)
            {
                printf("malloc() failed!\n");
                exit(EXIT_FAILURE);
            }
            memset(hostcopy, 0, strlen("http:")+strlen(url)+1);
            strcat(hostcopy, "http:");
        }
        else
        {
            //Make a deep copy of hostname
            hostcopy = malloc((strlen(hostname) + strlen(url) + 1)*sizeof(char));
            if (!hostcopy)
            {
                printf("malloc() failed!\n");
                exit(EXIT_FAILURE);
            }
            memset(hostcopy, 0, strlen(hostname)+strlen(url)+1);
            strcpy(hostcopy, hostname);
            rem_trail_slash(hostcopy);
            //Remove last component of url for appending relative url
            for (int i=strlen(hostcopy)-1;i>=0;i--)
            {
                if (hostcopy[i] != '/')
                {
                    hostcopy[i] = '\0';
                }
                else
                {
                    break;
                }
            }
            //Checks if url is of form /b.html
            if (url[0] == '/')
            {
                rem_trail_slash(hostcopy);
            }
        }

        strcat(hostcopy, url);
        strcpy(url, hostcopy);
        url[strlen(url)-1] = '\0';
    }
    else
    {
        printf("Bad regex!\n");
        exit(EXIT_FAILURE);
    }

    rem_trail_slash(url);
    htm_to_html(url);

    return 0;
}

/*
 *Uses regex to determine if url is a valid url 
 *based on the specs (no ?,#,%,./,../)
*/
int parse_valid_url(char** url_list)
{
    regex_t regex, regex2;
    int check, status, check2, status2;


    for (int i = 0; url_list[i][0] != '\0'; i++)
    {
        check = regcomp(&regex, "?", 0);
        check2 = regcomp(&regex2, NON_VALID_URL_REGEX, REG_EXTENDED);
    
        if (check || check2)
        {
            printf("\nCould not compile regex!\n");
            exit(EXIT_FAILURE);
        }

        status = regexec(&regex, url_list[i], 0, NULL, 0);
        status2 = regexec(&regex2, url_list[i], 0, NULL, 0);

        if (!status || !status2)
        {
            url_list[i][0] = '\0';
        }
        else if (status == REG_NOMATCH || status2 == REG_NOMATCH)
        {
            continue;
        }
        else
        {
            printf("ERROR REGEX\n");
            exit(EXIT_FAILURE);    
        }

        regfree(&regex);
        regfree(&regex2);
    }

    return 0;
}

/*
 *Checks if a given url has been visited already
 *Returns 0 if new url, 1 if already visited
*/
int visited(char** list, char* url, int max_url_num)
{
    regex_t regex;
    int status;

    regcomp(&regex, url, REG_EXTENDED);
    for (int i = 0; i < max_url_num; i++)
    {
        if (list[i][0] != '\0')
        {
            //URL matched visited list
            if (!(status=regexec(&regex, list[i], 0, NULL, 0)))
            {
                regfree(&regex);
                return 1;
            }
            continue;
        }
        list[i] = url;
        break;
    }
    regfree(&regex);
    
    return 0;
}

int rem_precede_slash(char* text)
{
    while(text[0] == '/')
    {
        text++;
    }
    return 0;
}

/*
 *Removes all trailing '/' from a given text
 */
int rem_trail_slash(char* text)
{
    for (int i=strlen(text)-1; i >= 0; i--)
    {
        if (text[i] == '/')
        {
            text[i] = '\0';
        }
        else
        {
            break;    
        }
    }
    return 0;
}

/*
 *Changes .htm to .html if any
 */
int htm_to_html(char* url)
{
    int str_end;

    str_end = strlen(url)-1;
    if(url[str_end]=='m' && url[str_end-1]=='t'
    && url[str_end-2]=='h')
    {
        url[str_end+1] = 'l';
        url[str_end+2] = '\0';
    }
    return 0;
}