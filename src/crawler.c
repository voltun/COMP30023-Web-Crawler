#include <stdio.h>
#include <string.h>
#include "include/client.h"

#define MAX_URL_LEN 1000
#define MAX_REQUEST_LEN 1000
#define CLI_INDEX_URL 1
#define REQUEST_GET "GET HTTP/1.1\r\n"
#define REQUEST_USERAGENT "User-Agent: chungw4\r\n"

int main(int argc, char const *argv[])
{
    char input_url[URL_MAX_LEN] = {0};
    char request_head[MAX_REQUEST_LEN] = {0};

    //Init vars
    input_url = malloc(MAX_URL_LEN*sizeof(char));
    if (!input_url)
    {
        printf("\nmalloc() failed!\n");
        return -1;
    }
    memset(input_url, '\0', MAX_URL_LEN);

    request_head = malloc(MAX_REQUEST_LEN*sizeof(char));
    if (!request_head)
    {
        printf("\nmalloc() failed!\n");
        return -1;
    }
    memset(request_head, '\0', sizeof(request_head));

    //Take input url from command line argument
    if (argc <= 1)
    {
        printf("No URL provided as input!\n");
        return -1;
    }
    else
    {
        strncpy(input_url, argv[CLI_INDEX_URL],
        sizeof(argv[CLI_INDEX_URL]));
    }

    //Connect to url via socket


    //Store url response


    //Parse html response


    //Redirect to anchored url


    //stdout redirects
    

    //Garbage collection
    free(input_url);

    return 0;
}