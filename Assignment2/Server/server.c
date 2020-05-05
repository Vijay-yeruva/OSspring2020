/* Program: server.c
 * A simple server TCP sockets.
 * Server is executed before Client.
 * Port number is to be passed as an argument. 
 * 
 * To test: Open a terminal window.
 * At the prompt ($ is my prompt symbol) you may
 * type the following as a test:
 *
 * $ ./server 54554
 * Run client by providing host and port
 *
 * 
 */
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>

#define BUFF_LEN 256
#define MAX_TOKEN 10

#define FAILURE 1
#define SUCCESS 0

#define FALSE 0
#define TRUE 1

#define READ "r+"
#define WRITE "w"

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int parseRequest(char *request, char **command, char *ch, char **filename)
{
    char *cmd = *command;
    char *file = *filename;
    **command = '\0';
    *ch = '\0';
    **filename = '\0';

    while (request != NULL && *request != '\0')
    {
        if (*request != ' ')
        {
            *cmd = *request;
            cmd++;
            request++;
        }
        else
        {
            break;
        }
    }
    cmd++;
    *cmd = '\0';
    if (strcmp(*command, "toUpper") == SUCCESS)
    {
        //Need to read only the file name
        while (request != NULL && *request != '\0')
        {
            if (*request != ' ' && *request != '<' 
             && *request != '>' && *request != '\n')
            {
                *file = *request;
                file++;
                request++;
            }
            else
            {
                request++;
                continue;
            }
        }
    }
    else if (strcmp("count", *command) == SUCCESS)
    {
        //Need to read char
        while (request != NULL && *request != '\0')
        {
            if (*request != ' ' && *request != '<' && *request != '>' && *request != ',')
            {
                *ch = *request;
                request++;
            }
            else if (*request == ',')
            {
                request++;
                break;
            }
            else
            {
                request++;
                continue;
            }
        }
        //file name
        while (request != NULL && *request != '\0')
        {
            if (*request != ' ' && *request != '<' 
             && *request != '>' && *request != '\n')
            {
                *file = *request;
                file++;
                request++;
            }
            else
            {
                request++;
                continue;
            }
        }
    }
    else
    {
        return FAILURE;
    }

    return SUCCESS;
}

int main(int argc, char *argv[])
{
    int sockfd, newsockfd, portno, total = 0;
    socklen_t clilen;
    char buffer[BUFF_LEN], ch, *command, *filename;
    struct sockaddr_in serv_addr, cli_addr;
    int n;
    FILE* fp;

    memset(buffer, '\0', BUFF_LEN);

    if (argc < 2)
    {
        fprintf(stderr, "ERROR, no port provided\n");
        exit(1);
    }

    fprintf(stdout, "Run client by providing host and port\n");
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("ERROR opening socket");
    bzero((char *)&serv_addr, sizeof(serv_addr));
    portno = atoi(argv[1]);

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);
    if (bind(sockfd, (struct sockaddr *)&serv_addr,
             sizeof(serv_addr)) < 0)
        error("ERROR on binding");

    listen(sockfd, 5);
    clilen = sizeof(cli_addr);
    newsockfd = accept(sockfd,
                       (struct sockaddr *)&cli_addr,
                       &clilen);
    if (newsockfd < 0)
        error("ERROR on accept");
    bzero(buffer, BUFF_LEN);
    n = read(newsockfd, buffer, BUFF_LEN - 1);
    if (n < 0)
    {
        error("ERROR reading from socket");
    }
    else
    {
        command = (char *)malloc(MAX_TOKEN);
        memset(command, '\0', MAX_TOKEN);
        filename = (char *)malloc(BUFF_LEN);
        memset(filename, '\0', BUFF_LEN);

        parseRequest(buffer, &command, &ch, &filename);
        printf("command: %s\n", command);
        printf("Char: %c\n",ch);
        printf("filename: %s\n", filename);

        if (n < 0)
            error("ERROR writing to socket");
        if (strcmp("toUpper", command) == SUCCESS)
        {
            fp = fopen(filename, READ);
            memset(buffer, '\0', BUFF_LEN);
            if(fp == NULL)
            {
                printf("Error opening file\n");
            }
            while (fgets(buffer, BUFF_LEN - 1, fp) != NULL)
            {       
                for (n = 0; n < BUFF_LEN - 1; n++)
                {
                    if(buffer[n] >= 97 && buffer[n] <= 123)
                    buffer[n] = buffer[n] - 32;
                }
                printf("Upper: %s\n", buffer); 
                n = write(newsockfd, buffer, strlen(buffer));
                if (n < 0)
                    error("ERROR writing to socket");
                memset(buffer, '\0', BUFF_LEN);
            }
            fclose(fp);
        }

        if (strcmp("count", command) == SUCCESS)
        {
            fp = fopen(filename, READ);
            int total = 0;
            while (fgets(buffer, BUFF_LEN, fp) != NULL)
            {
                printf("%s\n",buffer);
                for (n = 0; n < BUFF_LEN && buffer[n] != '\0'; n++)
                {
                    if(buffer[n] == ch)
                    total++;
                }
                memset(buffer, '\0', BUFF_LEN);
            }
            memset(buffer, '\0', BUFF_LEN);
            sprintf(buffer, "%d", total);
            printf("Total count: %d\n", total);
            n = write(newsockfd, buffer, BUFF_LEN);
            fclose(fp);
        }
    }

    close(newsockfd);
    close(sockfd);
    return 0;
}
