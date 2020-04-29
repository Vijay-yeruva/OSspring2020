/******************************************************************************
* 
* Name: 	Vijay Yeruva
* Email: 	vyeruva@albany.edu
*
* helper.c 
* Contains all the helper methods for the shell
*
******************************************************************************/

#include "helper.h"

/******************************************************************************
* Prints the device's username along with current directory
******************************************************************************/
void print_user()
{
	char path[MAX_LEN];
	getcwd(path, MAX_LEN);
	char* username = getenv("USER");
	printf("%s@Directory:%s> ", getenv("USER"), path);
}

/******************************************************************************
* Gets character
******************************************************************************/
char getch(FILE *fp)
{
	if (fp != NULL)
	{
	  return fgetc(fp);
	}
	char ch = getchar();
  	return ch;
}

/******************************************************************************
* deletes the current character.
******************************************************************************/
void delete(int len, char* line)
{
	line[len] = '\0';
	printf("\b \b");
}

/******************************************************************************
* Determines which special character is processed.
******************************************************************************/
char switch_keypad(char ch)
{
	switch(ch)
	{
		case 'A': return KEYUP;
		case 'B': return KEYDOWN;
		case 'C': return KEYRIGHT;
		case 'D': return KEYLEFT;
		default:  return ch;
	}
}

/******************************************************************************
* Copies from null terminated buffer to destination
******************************************************************************/
int copybuff(char *buffer, char **ptrdestination)
{
	int len = strlen(buffer);
	if (len > 0)
	{
		char *destination = *ptrdestination;
		destination = (char *)malloc(len + 1);
		strcpy(destination, buffer);
		destination[len] = '\0';
		*ptrdestination = destination;
	}
	return len;
}

/******************************************************************************
* opens a file with given file path
******************************************************************************/
FILE* openfile(char* filename)
{
	FILE *fp = NULL;

    if (filename != NULL)
    {
        fp = fopen(filename, READ);

        if (fp == NULL)
            printf("Unable to open %s\n", filename);
    }
	return fp;
}

/******************************************************************************
* closes the given file
******************************************************************************/
void closefile(FILE* fp)
{
	if(fp != NULL)
	{
		fclose(fp);
	}
}

/******************************************************************************
* function to trim space at the end of strings
******************************************************************************/
void trimtrailingspace(char** ptrline)
{
    char* line = *ptrline;
    int len = strlen(line);
    while(len > 0)
    {
        len--;
        if (line[len] == ' ')
        {
            line[len] = '\0';
        }
        else
        {
            break;
        }
    }
}
