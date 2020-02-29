/******************************************************************************
* 
* Name: 	Vijay Yeruva
* Email: 	vyeruva@albany.edu
*
* shell.c 
* Processes the users input, passes it to a tokenizer, and then passes
* the tokens obtained to the process_commands function which will process and
* execute the commands.
*
******************************************************************************/

#include "shell.h"

/******************************************************************************
* Processes the input and determine whether it is a user interface operation 
* or a set of commands that will need to be executed.
******************************************************************************/
void shell(char* filename)
{

    /**************************************************************************
    * short			special_char 	determines whether the character to be
    *					processed is special or not.
    * int 			len 		keeps track of the current line length
    * char 			line 		holds current line
    **************************************************************************/
    short special_char = FALSE;
    int status, len = 0;
    char ch, *line = (char*)malloc(MAX_LEN);
    memset(line, '\0', MAX_LEN);
    FILE *fp = NULL;

    if (filename != NULL)
    {
        fp = fopen(filename, READ);

        if (fp == NULL) printf("Unable to open %s\n", filename);

    }

    /**************************************************************************
    * Loops until the user exits the program.
    **************************************************************************/
    print_user();
    while (TRUE)
    {
        ch = getch(fp);
        if (special_char)
        {
            special_char = FALSE;
            ch = switch_keypad(ch);
        }

        /**********************************************************************
        * Switchs arround possible cases depending on the read character
        **********************************************************************/
        switch (ch)
        {
            /******************************************************************
            * Handles the ascii translation of arrow characters
            ******************************************************************/
        case '\033':
            getch(NULL);
            special_char = TRUE;
            continue;
            break;

            /******************************************************************
            * Ignore arrow characters and tab
            ******************************************************************/
        case KEYLEFT:
        case KEYRIGHT:
        case KEYUP:
        case KEYDOWN:
        case '\t':
            break;

            /******************************************************************
            * Handles backspacing
            ******************************************************************/
        case DELETE:
        {
            //if line length is more than zero then it is already 
            if (len > 0)
                len--;
            delete(len, line);
            break;
        }

        /******************************************************************
        * If the maximum line length is not exceeded the program will print
        * the character. If the character is not a new line then continue.
        * Else, terminate line, pass it to the execute_commands method,
        * allocate a new line, and reset the line length.
        ******************************************************************/
        default:
        {
            if (len < MAX_LEN)
            {
                if (ch != '\n')
                {
                    line[len] = ch;
                    len++;
                }
                else if (ch == '\n' && len > 0)
                {
                    execute_commands(line);
                    print_user();
                }
            }
            break;
        }
        }
    }

    fclose(fp);
}

/******************************************************************************
* The execute_commands function will process and execute the commands in 
* the variable line.
******************************************************************************/
short execute_commands(char* line)
{
    short status = SUCCESS;
    char* commands[15];
    int ncoms = parseCommands(line, commands);
    int i = 0;
    for (i = 0; i < ncoms; i++)
    {
        execute_command(commands[i]);
    }
    return status;
}

int parseCommands(char* line, char** commands)
{
    char buff[BUFF_LEN] = { '\0' };
    char* p = buff;
    int i = 0;
    while (*line != '\0')
    {
        if ((*line == ' ' && *(line + 1) != '-') || *line == '|' || *line == '<' || *line == '>' || *line == '&')
        {
            if (strlen(buff) == 0)
            {
                line++;
                continue;
            }
            commands[i] = (char*)malloc(strlen(buff));
            strcpy(commands[i], buff);
            i++;
            if (*line == '|' || *line == '<' || *line == '>' || *line == '&')
            {
                memset(buff, '\0', BUFF_LEN);
                p = buff;
                *p = *line;
                line++;
                p++;
                commands[i] = (char*)malloc(strlen(buff));
                strcpy(commands[i], buff);
                i++;
            }
            else
            {
                line++;
            }
            memset(buff, '\0', BUFF_LEN);
            p = buff;
        }
        else
        {
            *p = *line;
            line++;
            p++;
        }
    }
    if (strlen(buff) != 0)
    {
        commands[i] = (char*)malloc(strlen(buff));
        strcpy(commands[i], buff);
        i++;
    }
    return i;
}

short execute_command(char* cmd)
{
    short status = SUCCESS;
    int result;
    pid_t child_pid;
    int status;

    // Fork the child process
    child_pid = fork();

    // Check for errors in fork()
    switch (child_pid) {
    case EAGAIN:
        //Error due to I/O
        perror("Error EAGAIN: ");
        return 0;
    case ENOMEM:
        //Not able to allocate memory error
        perror("Error ENOMEM: ");
        return 1;
    }

    if (child_pid == 0) {

    }
    return status;
}