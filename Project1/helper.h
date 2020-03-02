/******************************************************************************
* 
* Name: 	Your Name
* Email: 	Your email
*
* helper.h
*
******************************************************************************/

#ifndef HELPER_H
#define HELPER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <unistd.h>

#define MAX_LEN 1024
#define BUFF_LEN 256 
#define MAX_TOKEN 10
#define TERMINATE -1
#define FAILURE 1
#define SUCCESS 0

#define FALSE 0
#define TRUE 1
#define DELETE 127
#define KEYLEFT 4
#define KEYRIGHT 5
#define KEYUP 3
#define KEYDOWN 2
#define READ "r"
#define WRITE "w"

void print_user();
char getch(FILE*);
void delete(int, char*);
char switch_keypad(char);
int copybuff(char*, char**);
FILE* openfile(char*);
void closefile(FILE* fp);
void trimtrailingspace(char** ptrline);

#endif
