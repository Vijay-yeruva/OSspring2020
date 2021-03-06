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
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include "helper.h"

#define PIPE '|'
#define INPUT '<'
#define OUTPUT '>'
#define BKGPROCESS '&'

void shell(char*);
short execute_commands(char*);
short execute_command(char**, char*,int**);
int parseCommand(char**, char**, char*);
int tokenize_cmd(char*, char**);
short awaitChildren();

#endif
