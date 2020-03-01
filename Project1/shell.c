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
void shell(char *filename)
{

    /**************************************************************************
    * short			special_char 	determines whether the character to be
    *					processed is special or not.
    * int 			len 		keeps track of the current line length
    * char 			line 		holds current line
    **************************************************************************/
    short special_char = FALSE;
    int status, len = 0;
    char ch, *line = (char *)malloc(MAX_LEN);
    memset(line, '\0', MAX_LEN);
    FILE *fp = openfile(filename);

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
            delete (len, line);
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
                else 
                {
                    if (execute_commands(line) != TERMINATE)
                    {
                        print_user();
                    }
                    else
                    {
                        closefile(fp);
                        return;
                    }
                }
            }
            break;
        }
        }
    }

    closefile(fp);
}

/******************************************************************************
* The execute_commands function will process and execute the commands in 
* the variable line.
******************************************************************************/
short execute_commands(char *line)
{
    short status = SUCCESS;
    char *command;
    char end = '\0';
    int len = parseCommand(line, &command, &end);

    if (len > 0)
    {
        if (end == INPUT)
        {
            char *fin;
            parseCommand(line, &fin, &end);
            FILE* fp = freopen(fin, READ, stdin);
            if(fp == NULL)
            {
                printf("Input file does not exist\n");
                return FAILURE;
            }
        }

        if (end == OUTPUT)
        {
            char *fout;
            parseCommand(line, &fout, &end);
            FILE* fp = freopen(fout, WRITE, stdout);
            if(fp == NULL)
            {
                printf("Not able to open output file\n");
                return FAILURE;
            }
        }

        status = execute_command(command, &end);
        if (status == SUCCESS)
        {
            status = execute_commands(line);
        }
        if (end != BKGPROCESS && status == SUCCESS)
        {
            status = awaitChildren();
        }
    }

    return status;
}

int parseCommand(char *line, char **command, char *end)
{
    char buff[BUFF_LEN] = {'\0'};
    char *p = buff;
    while (*line != '\0')
    {
        if (*line == PIPE || *line == INPUT || *line == OUTPUT || *line == BKGPROCESS)
        {
            *end = *line;
            break;
        }
        else
        {
            *p = *line;
            line++;
            p++;
        }
    }
    copybuff(buff, command);
    return strlen(buff);
}

short execute_command(char *cmd, char *end)
{
    int result, fds[2];
    pid_t child_pid;
    char *tokens[MAX_TOKEN];
    int tcount = tokenize_cmd(cmd, tokens);

    if (strncmp(tokens[0], "exit", 4) == SUCCESS)
        return TERMINATE;

    if (strncmp(tokens[0], "cd", 2) == SUCCESS)
    {
        printf("changing Directory to:%s\n", tokens[1]);
        if(tcount > 1)
        {
            chdir(tokens[1]);
        }
        return SUCCESS;
    }

    if (*end == PIPE)
    {
        if (pipe(fds) < 0)
        {
            perror("pipe failed");
            return FAILURE;
        }
    }
    // Fork the child process
    child_pid = fork();

    // Check for errors in fork()
    switch (child_pid)
    {
    case EAGAIN:
        //Error due to I/O
        perror("Error EAGAIN");
        return FAILURE;
    case ENOMEM:
        //Not able to allocate memory error
        perror("Error ENOMEM");
        return FAILURE;
    case ENOSYS:
        //fork is not suppoted on this system
        perror("Error ENOSYS");
        return FAILURE;
    }

    if (child_pid == 0)
    {
        if (*end == PIPE)
        {
            //Set up redirection in the child process
            close(fds[0]);
            if (dup2(fds[1], STDOUT_FILENO) < 0)
            {
                perror("can't dup");
                exit(1);
            }
            close(fds[1]);
        }
        // Execute the command
        result = execvp(tokens[0], tokens);
        perror("execlp\n");
        exit(FAILURE);
    }
    else
    {
        //redirect the pipe to std for next command
        if (*end == PIPE)
        {
            close(fds[1]);
            if (dup2(fds[0], STDIN_FILENO) < 0)
            {
                perror("can't dup");
                exit(1);
            }
            close(fds[0]);
        }
    }
    
    return SUCCESS;
}

int tokenize_cmd(char *cmd, char **tokens)
{
    char buff[BUFF_LEN] = {'\0'};
    char *p = buff;
    int i = 0;
    while (*cmd != '\0')
    {
        if (*cmd == ' ' && *(cmd + 1) != ' ')
        {
            copybuff(buff, &tokens[i]);
            memset(buff, '\0', BUFF_LEN);
            p = buff;
            i++;
            cmd++;
        }
        else if (*cmd == ' ')
        {
            cmd++;
        }
        else
        {
            *p = *cmd;
            cmd++;
            p++;
        }
    }
    copybuff(buff, &tokens[i]);
    i++;
    return i;
}

short awaitChildren()
{
    pid_t cpid;
    while(1) {
        if ((cpid = wait(NULL)) == -1){
            if (errno == ECHILD) {
                return SUCCESS;
            } else {
                perror("wait error");
                return FAILURE;
            }
        }
    }
}