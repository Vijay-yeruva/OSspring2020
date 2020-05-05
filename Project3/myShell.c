/******************************************************************************
* 
* Name: 	Vijay Yeruva
* Email: 	vyeruva@albany.edu
*
* myShell.c is the entry point to your shell program
* 
*
******************************************************************************/

#include "myShell.h"

/******************************************************************************
* Main function.
******************************************************************************/
int main(int argc, char** argv)
{
     if (argc > 1 && !strcmp(argv[1], SHELL))
    {
        FILE *fp = openfile(argv[2]);
        shell(fp);
    }
    else
    {
        printf("\nUsage:\t./myOS COMMAND\n\nAvailable commands:\n\n\t1. shell\n\n");
    }
    return SUCCESS;
}
