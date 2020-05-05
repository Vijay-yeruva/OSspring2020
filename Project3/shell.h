/******************************************************************************
* 
* Name: 	Vijay Yeruva
* Email: 	vyeruva@albany.edu
*
* shell.h
*
******************************************************************************/

#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#include "helper.h"

#define PIPE '|'
#define INPUT '<'
#define OUTPUT '>'
#define BKGPROCESS '&'
#define ATADDRESS '@'
#define SERVER_PNO 1026

void shell(FILE *fp);
short execute_commands(char*);
short execute_command(char**, char*,int**);
int parseCommand(char**, char**, char*);
int tokenize_cmd(char*, char**);
short awaitChildren();
int startserver(int, int*);
int startclient(char*, int*);

#endif
