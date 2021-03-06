#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <strings.h>

#define PORT 80
#define MAX_BUFFER_LEN 1024
#define MAX_STATUS_CODE_LEN 3
#define STATUS_200 "200"
#define HTTP_LEN "HTTP/1.1 "
#define REQUEST_GET "GET " 
#define REQUEST_GET_HTTP " HTTP/1.1\r\n"
#define REQUEST_HOST_TEMP "Host: "
#define REQUEST_USERAGENT "User-Agent: chungw4\r\n"
#define REQUEST_ACCEPT "Accept: text/html, text/plain\r\n"

int connect_to(char* url, char* request, char* raw_response, int max_response_len)
{
    int sock = 0, valread, n;
    char statuscode[MAX_STATUS_CODE_LEN];
    struct hostent *server = NULL;
    char response_header[MAX_BUFFER_LEN];
    struct sockaddr_in serv_addr;
    char buffer[MAX_BUFFER_LEN];

    //Init char arrays
    bzero(buffer, MAX_BUFFER_LEN);
    bzero(response_header, MAX_BUFFER_LEN);
    bzero(raw_response, max_response_len);
    bzero(statuscode, MAX_STATUS_CODE_LEN);

    //Resolve hostname
    server = gethostbyname(url);

    if(server == NULL)
    {
        printf("Server not found: %s\n", url);
        raw_response = NULL;
        return 0;
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

    //Read response
    valread = read(sock, raw_response, max_response_len - 1);

    if (valread < 0)
    {
        printf("Error reading from socket!\n");
        exit(EXIT_FAILURE);
    }

    for (int j = 0; j < 3; j++)
    {
        statuscode[j] = raw_response[strlen(HTTP_LEN) + j];
    }

    //Bad status code
    if (strcmp(statuscode, STATUS_200) != 0)
    {
        raw_response = NULL;
    }

    return 0;
}

int create_request_header(char* request_head, char* hostname, char* uri)
{
    char* host = NULL;
    char* get = NULL;

    host = (char *)malloc((strlen(REQUEST_HOST_TEMP) + 
    strlen(hostname) + strlen("\r\n") + 1)*sizeof(char));
    if (!host)
    {
        printf("\nmalloc() failed!\n");
        return -1;
    }
    memset(host, 0, strlen(REQUEST_HOST_TEMP)+strlen(hostname)+strlen("\r\n")+1);

    get = malloc((strlen(REQUEST_GET)+strlen(uri)+strlen(
        REQUEST_GET_HTTP)+1)*sizeof(char));
    if (!get)
    {
        printf("\nmalloc() failed!\n");
        return -1;
    }
    memset(get, 0, strlen(REQUEST_GET)+strlen(uri)+strlen(
        REQUEST_GET_HTTP)+1);
    
    //Prepare the GET header field
    strcpy(get, REQUEST_GET);
    strcat(get, uri);
    strcat(get, REQUEST_GET_HTTP);
    strcpy(request_head, get);

    //Prepare Host header field
    strcpy(host, REQUEST_HOST_TEMP);
    strcat(host, hostname);
    strcat(host, "\r\n");

    //Prepare User-Agent and Accept header field
    strcat(request_head, host);
    strcat(request_head, REQUEST_USERAGENT);   
    strcat(request_head, REQUEST_ACCEPT);
    strcat(request_head, "\r\n\r\n");

    free(host);
    free(get);

    return 0;
}

