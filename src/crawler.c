#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../include/client.h"
#include "../include/parser.h"

#define MAX_URL_LEN 1000
#define MAX_REQUEST_LEN 1000
#define MAX_BUFFER_LEN 100000
#define CLI_INDEX_URL 1
#define REQUEST_GET "GET HTTP/1.1\r\n"
#define REQUEST_USERAGENT "User-Agent: chungw4\r\n"

int main(int argc, char const *argv[])
{
    char* input_url = NULL;
    char* request_head = NULL;
    char* raw_response = NULL;

    printf("check 0");

    //Init vars
    input_url = (char *)malloc(MAX_URL_LEN*sizeof(char));
    if (!input_url)
    {
        printf("\nmalloc() failed!\n");
        return -1;
    }
    memset(input_url, 0, sizeof(input_url));

    printf("check 1");

    request_head = (char *)malloc(MAX_REQUEST_LEN*sizeof(char));
    if (!request_head)
    {
        printf("\nmalloc() failed!\n");
        return -1;
    }
    memset(request_head, 0, MAX_REQUEST_LEN*sizeof(char));

    raw_response = (char *)malloc(MAX_BUFFER_LEN*sizeof(char));
    if (!raw_response)
    {
        printf("\nmalloc() failed!\n");
        return -1;
    }
    memset(raw_response, 0, MAX_BUFFER_LEN*sizeof(char));

    //Take input url from command line argument
    if (argc <= 1)
    {
        printf("No URL provided as input!\n");
        return -1;
    }
    else
    {
        strncpy(input_url, argv[CLI_INDEX_URL],
        strlen(argv[CLI_INDEX_URL]));
    }

    //Create request header
    strncpy(request_head, REQUEST_GET, MAX_REQUEST_LEN - 
    strlen(request_head));

    strncpy(request_head, strcpy("Host: ", input_url), MAX_REQUEST_LEN
    - strlen(request_head));

    strncpy(request_head, REQUEST_USERAGENT, MAX_REQUEST_LEN -
    strlen(request_head));

    printf("%s\n", request_head);

    //Connect to url via socket
    

    //Store url response


    //Parse html response


    //Redirect to anchored url


    //stdout redirects
    

    //Garbage collection
    free(input_url);

    return 0;
}