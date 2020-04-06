#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

#define PORT 8080
#define REQUEST_MAX_LEN 1000
#define BUFFER_MAX_LEN 1024

string connect_to(char* url)
{
    int sock = 0, valread;
    struct sockaddr_in serv_addr;
    char request[REQUEST_MAX_LEN] = {0};
    char buffer [BUFFER_MAX_LEN] = {0};

    //Create request


    //Create socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
    {
        printf("\nSocket creation error!\n");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    //Convert url to IPv4
    if (inet_pton(AF_INET, url, &serv_addr.sin_addr) <= 0)
    {
        printf("\nInvalid URL address!\n");
        return -1;
    }

    //Open connection
    if (connect(sock, (struct sockaddr *)&serv_addr, 
    sizeof(serv_addr)) < 0)
    {
        printf("\nConnection Failed!\n");
        return -1;
    }

    //Send request


    //Read response


    //Return reponse
}