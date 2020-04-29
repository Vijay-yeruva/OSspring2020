/******************************************************************************
* 
* Name: 	Vijay Yeruva
* Email: 	vyeruva@albany.edu
*
* helper.h
*
******************************************************************************/

#ifndef HELPER_H
#define HELPER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_LEN 1024
#define BUFF_LEN 256 
#define MAX_TOKEN 10

#define FAILURE 1
#define SUCCESS 0

#define FALSE 0
#define TRUE 1

#define READ "r+"
#define WRITE "w"
#define CMD_TOUPPER "toUpper"
#define CMD_COUNT "count"

int parseRequest(char* request, char** command, char* ch, char** filename);

#endif
