/******************************************************************************
* 
* Name: 	Your Name
* Email: 	Your email
*
* helper.c 
* Contains all the helper methods for the shell
*
******************************************************************************/

#include "helper.h"

/******************************************************************************
* Prints the device's username
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
* Copyies from null terimated buffer to destination
******************************************************************************/
int copybuff(char *buffer, char **destination)
{
	int len = strlen(buffer);
	*destination = (char *)malloc(len + 1);
	strcpy(*destination, buffer);
	(*destination)[len] = '\0';
	return len;
}

/******************************************************************************
* open a file with given file path
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