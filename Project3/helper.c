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

/******************************************************
* Function writes error message
*******************************************************/
void error(const char *msg)
{
    perror(msg);
    exit(1);
}

/******************************************************
* Function writes from STDIN to given file descriptor
*******************************************************/
void startwriting(int fd)
{
    char buffer[BUFF_LEN] = {'\0'};
	int n = 0;
    while (TRUE)
    {
        char ch;
        int len = 0;

        printf("Enter command: ");
        while (TRUE)
        {
            ch = getchar();
            if (len < BUFF_LEN-1)
            {
                if (ch != '\n')
                {
                    buffer[len] = ch;
                    len++;
                }
                else
                {
                    if (strncmp(buffer, "exit", 4) == SUCCESS)
                    {
                        break;
                    }
                    else
                    {
                        buffer[len] = '\n';
                        n = write(fd, buffer, strlen(buffer));
                        if (n < 0)
                            error("ERROR writing to socket");
                        memset(buffer, '\0', BUFF_LEN);
                        len = 0;
                        break;
                    }
                }
            }
            else
            {
                printf("Exceeded max command length\n");
                memset(buffer, '\0', BUFF_LEN);
                len = 0;
                break;
            }
        }

        if (strlen(buffer) == 4 &&
            strncmp(buffer, "exit", 4) == SUCCESS)
        {
            break;
        }
    }
}

/******************************************************
* Function reads from given file descriptor and writes to STDOUT
*******************************************************/
void startreading(int fd)
{
    char buffer[BUFF_LEN] = {'\0'};
	int n = 0;
    while ((n = read(fd, buffer, BUFF_LEN - 1)) > 0)
    {
        buffer[n] = '\0';
        printf("%s\n", buffer);
    }
}