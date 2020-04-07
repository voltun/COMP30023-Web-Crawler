#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <strings.h>

#define PORT 8080
#define MAX_BUFFER_LEN 1024

int connect_to(char* url, char* request, char* raw_response, int max_response_len)
{
    int sock = 0, valread, n;
    struct hostent *server = NULL;
    struct sockaddr_in serv_addr;
    char buffer[MAX_BUFFER_LEN];

    //Init char arrays
    bzero(buffer, MAX_BUFFER_LEN);

    bzero(raw_response, max_response_len);

    //Resolve hostname
    server = gethostbyname(url);

    if(server == NULL)
    {
        printf("Server not found!\n");
        exit(EXIT_FAILURE);
    }

    //Prepare socket
    bzero((char *)&serv_addr, sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;

    bcopy(server->h_addr_list[0], (char *)&serv_addr.sin_addr.s_addr,
     server->h_length);

    serv_addr.sin_port = htons(PORT);

    //Create socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) 
    {
        printf("Failed to open socket!\n");
        exit(EXIT_FAILURE);
    }

    //Connect
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("Connection failed!\n");
        exit(EXIT_FAILURE);
    }

    //Write to socket
    n = write(sock, request, strlen(request));
    if (n < 0)
    {
        printf("Failed to write to socket!\n");
        exit(EXIT_FAILURE);
    }

    printf("hi 2");
    //Read response
    while((valread = read(sock, buffer, sizeof(buffer) - 1)) > 0)
     {
         if (((int)(strlen(buffer) + strlen(raw_response))) < max_response_len)
         {
             strcat(raw_response, buffer);
         }
         else
         {
             break;
         }
     }


    //Garbage collection

    return 0;
}
