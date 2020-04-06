#include <stdio.h>
#include <string.h>

#define URL_MAX_LEN 1000
#define CLI_INDEX_URL 1

int main(int argc, char const *argv[])
{
    char input_url[URL_MAX_LEN];

    //Init vars
    input_url = malloc(URL_MAX_LEN*sizeof(char));
    memset(input_url, '\0', URL_MAX_LEN);
    
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