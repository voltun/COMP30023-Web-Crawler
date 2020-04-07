#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../include/client.h"
#include "../include/parser.h"

#define MAX_URL_LEN 1000
#define MAX_REQUEST_LEN 1000
#define MAX_BUFFER_LEN 100000
#define CLI_INDEX_URL 1
#define REQUEST_GET "GET /index.html HTTP/1.1\r\n"
#define REQUEST_HOST_TEMP "Host: "
#define REQUEST_USERAGENT "User-Agent: chungw4\r\n"
#define REQUEST_ACCEPT "Accept: text/html, text/plain\r\n"
#define REQUEST_CONN "Connection: keep-alive\r\n"

int main(int argc, char const *argv[])
{
    char* input_url = NULL;
    char* request_head = NULL;
    char raw_response[MAX_BUFFER_LEN];
    char* host = NULL;

    //Init vars
    input_url = (char *)malloc(MAX_URL_LEN*sizeof(char));
    if (!input_url)
    {
        printf("\nmalloc() failed!\n");
        return -1;
    }
    memset(input_url, 0, MAX_URL_LEN);

    request_head = (char *)malloc(MAX_REQUEST_LEN*sizeof(char));
    if (!request_head)
    {
        printf("\nmalloc() failed!\n");
        return -1;
    }
    memset(request_head, 0, MAX_REQUEST_LEN);

    host = (char *)malloc(strlen(REQUEST_HOST_TEMP)*sizeof(char));
    if (!host)
    {
        printf("\nmalloc() failed!\n");
        return -1;
    }
    memset(host, 0, strlen(REQUEST_HOST_TEMP));
    strcpy(host, REQUEST_HOST_TEMP);

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

    //Create request header
    strcat(request_head, REQUEST_GET);

    rem_http(input_url);
    host = (char *)realloc(host, strlen(REQUEST_HOST_TEMP) + 
    strlen(input_url) + strlen("\r\n"));
    strcat(host, input_url);
    strcat(host, "\r\n");
    strcat(request_head, host);

    strcat(request_head, REQUEST_USERAGENT);
    
    strcat(request_head, REQUEST_ACCEPT);

    strcat(request_head, "\r\n\r\n");

    //Connect to url via socket
    connect_to(input_url, request_head, raw_response, MAX_BUFFER_LEN);

    //Store url response
    printf("%s\n", raw_response);
    printf("%s\n", input_url);

    //Parse html response


    //Redirect to anchored url


    //stdout redirects
    

    //Garbage collection
    free(input_url);
    free(host);
    free(request_head);

    return 0;
}