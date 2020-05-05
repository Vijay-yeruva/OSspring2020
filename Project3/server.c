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
#include "helper.h"

int main(int argc, char *argv[])
{
    int sockfd, newsockfd, portno, total = 0;
    socklen_t clilen;
    char buffer[BUFF_LEN], ch, *command, *filename;
    struct sockaddr_in serv_addr, cli_addr;
    int n;
    FILE *fp;

    memset(buffer, '\0', BUFF_LEN);

    if (argc < 2)
    {
        fprintf(stderr, "ERROR, no port provided\n");
        exit(1);
    }

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

    while (TRUE)
    {
        printf("Enter the mode of the server Read(R)/write(W): ");
        ch = getchar();
        if(ch == 'R'){
            startreading(newsockfd);
            break;
        }else if (ch == 'W'){
            startwriting(newsockfd);
            break;
        }    
        else{
            printf("Please enter correct mode");
        }
    }
           
    close(newsockfd);
    close(sockfd);
    return 0;
}
