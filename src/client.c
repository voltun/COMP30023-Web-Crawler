#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

#define PORT 8080
#define MAX_BUFFER_LEN 1024
#define MAX_IPV4_LEN 15

void hostname_to_ip(char* hostname, char* ip);

char* connect_to(char* url, char* request, int max_response_len)
{
    int sock = 0, valread;
    struct sockaddr_in serv_addr;
    char* buffer = NULL;
    char ipv4_addr[INET_ADDRSTRLEN];
    char* ret_response = NULL;

    //Init char arrays
    buffer = (char *)malloc(MAX_BUFFER_LEN*sizeof(char));
    if (!buffer)
    {
        printf("\nmalloc() failed!\n");
        exit(EXIT_FAILURE);
    }
    memset(buffer, 0, MAX_BUFFER_LEN*sizeof(char));

    ret_response = malloc(max_response_len*sizeof(char));
    if (!ret_response)
    {
        printf("\nmalloc() failed!\n");
        exit(EXIT_FAILURE);
    }
    memset(ret_response, 0, max_response_len*sizeof(char));

    //Create socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
    {
        printf("\nSocket creation error!\n");
        exit(EXIT_FAILURE);
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    hostname_to_ip(url, ipv4_addr);

    //Convert url to IPv4
    if (inet_pton(AF_INET, ipv4_addr, &serv_addr.sin_addr) <= 0)
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
         if (((int)(strlen(buffer) + strlen(ret_response))) < max_response_len)
         {
             strcat(ret_response, buffer);
         }
     }


    //Garbage collection
    free(buffer);

    //Return response
    return ret_response;
}


/*
Resolves host domain name to IPv4 address
*/
void hostname_to_ip(char* hostname, char* ip)
{
    struct addrinfo addr, *result, *p;
    struct in_addr **addr;
    int n;

    memset(&addr, 0, sizeof(struct addrinfo));

    addr.ai_family = AF_UNSPEC;
    addr.ai_socktype = SOCK_STREAM;
    
    if ((n = getaddrinfo(NULL, url, &addr, &result)) != 0)
    {
        printf("\nHostname not found!\n");
        exit(EXIT_FAILURE);
    }

    for (p = res; p != NULL; p = p->ai_next)
    {
        void *addr;
        if (p->ai_family == AF_INET)
        {
            strcpy(ip, )
        }
    }
}