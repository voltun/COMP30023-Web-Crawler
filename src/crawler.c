#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../include/client.h"
#include "../include/parser.h"

#define MAX_URL_LEN 1000
#define MAX_REQUEST_LEN 1000
#define MAX_BUFFER_LEN 100000
#define MAX_URL_NUM 100
#define CLI_INDEX_URL 1

char** crawl_to(char*);
void free_2d_char(char**, int);

int main(int argc, char const *argv[])
{
    char* input_url = NULL;
    char** url_list = NULL;

    //Init vars
    input_url = (char *)malloc(MAX_URL_LEN*sizeof(char));
    if (!input_url)
    {
        printf("\nmalloc() failed!\n");
        return -1;
    }
    memset(input_url, 0, MAX_URL_LEN);

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

    //Crawl first URL
    url_list = crawl_to(input_url);
    
    for (int i=0; url_list[i][0] != '\0'; i++)
    {
        printf("%s\n", url_list[i]);
    }
    //Redirect to anchored url


    //stdout redirects
    

    //Garbage collection
    free_2d_char(url_list, MAX_URL_NUM);
    free(input_url);

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
    char* request_head = NULL;
    char raw_response[MAX_BUFFER_LEN];

    //init vars
    request_head = (char *)malloc(MAX_REQUEST_LEN*sizeof(char));
    if (!request_head)
    {
        printf("\nmalloc() failed!\n");
        exit(EXIT_FAILURE);
    }
    memset(request_head, 0, MAX_REQUEST_LEN);

    //Create request header
    rem_http(url);
    domain_name = split_hostname(url);
    create_request_header(request_head, domain_name, url);

    //Connect to url via socket and store response
    connect_to(domain_name, request_head, raw_response, MAX_BUFFER_LEN);

    //Store url response
    printf("%s\n", raw_response);
    
    //Garbage collection
    free(request_head);

    //Parse html response
    return parse_html(raw_response, MAX_URL_NUM, MAX_URL_LEN);

}

void free_2d_char(char** thing, int len)
{
    for (int i=0; i < len; i++)
    {
        free(thing[i]);
    }
}