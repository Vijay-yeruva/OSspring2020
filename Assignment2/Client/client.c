/*
 simple client to work with server.c program.
 * Host name and port used by server is to be
 * passed as arguments.
 *
 * To test: Open a terminal window.
 * At prompt ($ is my prompt symbol) you may
 * type the following as a test:
 *
 * $./client 127.0.0.1 54554
 * Please enter the message: Operating Systems is fun!  
 * I got your message
 * $ 
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#define BUFF_LEN 256
#define MAX_TOKEN 10

#define FAILURE 1
#define SUCCESS 0

#define FALSE 0
#define TRUE 1

#define READ "r+"
#define WRITE "w+"
#define CMD_TOUPPER "toUpper"
#define CMD_COUNT "count"

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

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[])
{
    int sockfd, portno, n;
    FILE *fp1, *fp2;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    char buffer[256];

    fp1 = fopen("intextUpper.txt", WRITE);
    fprintf(fp1, "After modifying to upper case:%s\n", "");
    fp2 = fopen("intextChar.txt", WRITE);
    fprintf(fp2, "Char count:%s\n", "");
    if (argc < 3)
    {
        fprintf(stderr, "usage %s hostname port\n", argv[0]);
        exit(0);
    }
    portno = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("ERROR opening socket");
    server = gethostbyname(argv[1]);
    if (server == NULL)
    {
        fprintf(stderr, "ERROR, no such host\n");
        exit(0);
    }
    bzero((char *)&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr_list[0],
          (char *)&serv_addr.sin_addr.s_addr,
          server->h_length);
    serv_addr.sin_port = htons(portno);

    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
        error("ERROR connecting");

    printf("Please enter the message: ");
    bzero(buffer, BUFF_LEN);
    fgets(buffer, BUFF_LEN - 1, stdin);
    char *command = (char *)malloc(MAX_TOKEN);
    memset(command, '\0', MAX_TOKEN);
    char ch = '\0';
    char *filename = (char *)malloc(BUFF_LEN);
    memset(command, '\0', BUFF_LEN);
    parseRequest(buffer, &command, &ch, &filename);
    
    n = write(sockfd, buffer, strlen(buffer));
    if (n < 0)
        error("ERROR writing to socket");
    bzero(buffer, BUFF_LEN);

    if (strcmp(command, CMD_TOUPPER) == SUCCESS)
    {
        if (fp1 == NULL)
        {
            printf("Error: opening file\n");
            return FAILURE;
        }

        printf("%s\n", "Reading from socket");
        while ((n = read(sockfd, buffer, BUFF_LEN - 1)) > 0)
        {
            buffer[n] = '\0';
            printf("%s\n", buffer);
            fprintf(fp1, "%s", buffer);
        }
    }

    if (strcmp(command, CMD_COUNT) == SUCCESS)
    {
        if (fp2 == NULL)
        {
            printf("Error opening file\n");
            return FAILURE;
        }

        while ((n = read(sockfd, buffer, BUFF_LEN)) > 0)
        {
            fprintf(fp2, "%s", buffer);
            printf("%s\n", buffer);
        }
    }
    fclose(fp1);
    fclose(fp2);
    close(sockfd);
    return 0;
}
