#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

#define PORT 8080
#define MAX_BUFFER_LEN 1024

char* connect_to(char* url, char* request, int max_response_len)
{
    int sock = 0, valread;
    struct sockaddr_in serv_addr;
    char buffer[MAX_BUFFER_LEN] = {0};
    char *ret_response = NULL;

    //Init char arrays
    buffer = (char *)malloc(MAX_BUFFER_LEN*sizeof(char));
    if (!buffer)
    {
        printf("\nmalloc() failed!\n");
        exit(EXIT_FAILURE);
    }
    memset(buffer, 0, sizeof(buffer));

    serv_addr = (struct sockaddr_in*)malloc(sizeof(sockaddr_in));
    if (!serv_addr)
    {
        printf("\nmalloc() failed!\n");
        exit(EXIT_FAILURE);
    }
    memset(&serv_addr, 0, sizeof(serv_addr));

    ret_response = malloc(max_response_len*sizeof(char));
    if (!ret_response)
    {
        printf("\nmalloc() failed!\n");
        exit(EXIT_FAILURE);
    }
    memset(ret_response, 0, sizeof(ret_response));

    //Create socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
    {
        printf("\nSocket creation error!\n");
        exit(EXIT_FAILURE);
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    //Convert url to IPv4
    if (inet_pton(AF_INET, url, &serv_addr.sin_addr) <= 0)
    {
        printf("\nInvalid URL address!\n");
        exit(EXIT_FAILURE);
    }

    //Open connection
    if (connect(sock, (struct sockaddr *)&serv_addr, 
    sizeof(serv_addr)) < 0)
    {
        printf("\nConnection Failed!\n");
        exit(EXIT_FAILURE);
    }

    //Send request
    send(sock, request, strlen(request), 0);

    //Read response
    while((valread = read(sock, buffer, sizeof(buffer) - 1)) > 0)
     {
         if (strlen(buffer) + strlen(ret_response) < max_response_len)
         {
             strcat(ret_response, buffer);
         }
     }


    //Garbage collection
    free(buffer);
    free(*serv_addr);

    //Return response
    return ret_response;
}