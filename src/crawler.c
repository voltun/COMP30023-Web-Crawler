#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../include/client.h"
#include "../include/parser.h"

#define MAX_URL_LEN 1000
#define MAX_REQUEST_LEN 1024
#define MAX_BUFFER_LEN 100000
#define MAX_URL_NUM 100
#define CLI_INDEX_URL 1

char** crawl_to(char*);
void free_2d_char(char**, int);

int main(int argc, char const *argv[])
{
    char* input_url = NULL;
    char** visited_url = NULL;
    char** ret_url = NULL;
    int count = 0;

    //Init vars
    input_url = (char *)malloc(MAX_URL_LEN*sizeof(char));
    if (!input_url)
    {
        printf("\nmalloc() failed!\n");
        return -1;
    }
    memset(input_url, 0, MAX_URL_LEN);

    visited_url = (char**)malloc(MAX_URL_NUM*sizeof(char*));
    for (int i = 0; i < MAX_URL_NUM; i++)
    {
        visited_url[i] = malloc(MAX_URL_LEN*sizeof(char));
        memset(visited_url[i], 0, MAX_URL_LEN);
    }
    
    //Take input url from command line argument
    if (argc == 1)
    {
        printf("No URL provided as input!\n");
        return -1;
    }
    else
    {
        strcpy(input_url, argv[CLI_INDEX_URL]);
    }

    //Insert CLI url into visited
    visited(visited_url, input_url, MAX_URL_NUM);

    do
    {
        count ++;
        int n = 0;
        char* urlcopy = malloc(MAX_URL_LEN*sizeof(char));
        bzero(urlcopy, MAX_URL_LEN);

        strcpy(urlcopy, input_url);
        ret_url = crawl_to(urlcopy);
        if (ret_url != NULL)
        {
            while(ret_url[n][0] != '\0')
            {
                visited(visited_url, ret_url[n], MAX_URL_NUM);                   
                n++;
            }
        }
        
        if(visited_url[count] == NULL || visited_url[count][0] == '\0')
        {
            free(urlcopy);
            break;   
        }
        input_url = visited_url[count];
    } while(1);

    //Garbage collection
    free_2d_char(visited_url, MAX_URL_NUM);
    free_2d_char(ret_url, MAX_URL_NUM);

    return 0;
}

/*
 * Resolves hostname, URI and opens connection to host, fetch response
 * and parse response into anchored url links
 * Returns char**, 2D array of URL parsed from server response
 */
char** crawl_to(char* url)
{
    char* domain_name = NULL;
    char* url_copy = NULL;
    char* request_head = NULL;
    char* raw_response = NULL;

    //init vars
    raw_response = (char *)malloc(MAX_BUFFER_LEN*sizeof(char));
    if (!raw_response)
    {
        printf("\nmalloc() failed!\n");
        exit(EXIT_FAILURE);
    }
    memset(raw_response, 0, MAX_BUFFER_LEN);

    request_head = (char *)malloc(MAX_REQUEST_LEN*sizeof(char));
    if (!request_head)
    {
        printf("\nmalloc() failed!\n");
        exit(EXIT_FAILURE);
    }
    memset(request_head, 0, MAX_REQUEST_LEN);

    url_copy = malloc(MAX_URL_LEN*sizeof(char));
    if (!url_copy)
    {
        printf("\nmalloc() failed!\n");
        exit(EXIT_FAILURE);
    }
    strcpy(url_copy, url);

    printf("%s\n", url);

    //Create request header
    rem_http(url);
    domain_name = split_hostname(url);
    create_request_header(request_head, domain_name, url);

    //Connect to url via socket and store response in request_head
    connect_to(domain_name, request_head, raw_response, MAX_BUFFER_LEN);
    
    free(request_head);
    free(domain_name);

    if (raw_response[0] == '\0')
    {
        return NULL;
    }
    //Parse html response
    return parse_html(raw_response, url_copy, MAX_URL_NUM, MAX_URL_LEN);

}

/*
 *Frees a 2D char array, duh
 */
void free_2d_char(char** thing, int len)
{
    for (int i=0; i < len; i++)
    {
        free(thing[i]);
    }
    free(thing);
}
