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
                        memset(line, '\0', MAX_LEN);
                        len = 0;
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
    char *command, *fin, *fout;
    char end = '\0';
    int *prevfds = NULL;
    status = execute_command(&line, &end, &prevfds);
    if (end != BKGPROCESS && status == SUCCESS)
    {
        status = awaitChildren();
    }
    return status;
}

/******************************************************************************
* The execute_command function will process and execute the commands recursively
* one after the other.
******************************************************************************/
short execute_command(char **line, char *end, int **ptrPrevfds)
{
    short status = SUCCESS;
    int result, *fds = NULL, *prevfds = *ptrPrevfds;
    pid_t child_pid;
    char *tokens[MAX_TOKEN], *command = NULL, *fin = NULL, *fout = NULL;
    int tcount = 0;
    int len = parseCommand(line, &command, end);
    //if there is command excute other wise return from here.
    if (len > 0)
    {
        //Now tokenize the given command to get its args
        tcount = tokenize_cmd(command, tokens);
        //Check for exit
        if (strncmp(tokens[0], "exit", 4) == SUCCESS)
            return TERMINATE;
        //check for change directory
        if (strncmp(tokens[0], "cd", 2) == SUCCESS)
        {
            if (tcount > 1)
            {
                chdir(tokens[1]);
            }
            return SUCCESS;
        }
        //check if there is redirection at the end of current command and
        //take note of the input and output files as needed
        if (*end == INPUT)
        {
            parseCommand(line, &fin, end);
            trimtrailingspace(&fin);
        }
        if (*end == OUTPUT)
        {
            parseCommand(line, &fout, end);
            trimtrailingspace(&fout);
        }
    }
    else
    {
        return SUCCESS;
    }

    //check if there is pipe at the end of the command
    if (*end == PIPE)
    {
        fds = (int *)malloc(2 * sizeof(int));
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
        //In the child do the necessary IO redirection
        if (fin != NULL)
        {
            FILE *fp = freopen(fin, READ, stdin);
            if (fp == NULL)
                perror("Cannot open input file");
        }

        if (fout != NULL)
        {
            FILE *fp = freopen(fout, WRITE, stdout);
            if (fp == NULL)
                perror("Cannot open output file");
        }
        //check if there is a pipe before this command
        //if there is first redirect it to std in
        if (prevfds != NULL)
        {
            close(prevfds[1]);
            if (dup2(prevfds[0], STDIN_FILENO) == -1)
            {
                perror("can't dup");
                exit(FAILURE);
            }
            close(prevfds[0]);
        }
        //check if there is a pipe at the end of the command
        //if there is then redirect the stout to pipe
        if (*end == PIPE)
        {
            close(fds[0]);
            if (dup2(fds[1], STDOUT_FILENO) == -1)
            {
                switch (errno)
                {
                case EBADF:
                    perror("dup2 bad file discriptor");
                    break;
                case EBUSY:
                    perror("dup2 busy opening file discriptor");
                    break;
                case EINTR:
                    perror("dup2 interrupted by signal");
                    break;
                case EMFILE:
                    perror("dup2 limit on open file discriptor");
                    break;
                default:
                    break;
                }
                exit(1);
            }
            close(fds[1]);
        }
        // Execute the command
        result = execvp(tokens[0], tokens);
        perror("execvp\n");
        return FAILURE;
    }
    else
    {
        //In parent process close all the previous pipes
        if (prevfds != NULL)
        {
            close(prevfds[0]);
            close(prevfds[1]);
        }
        //store the current pipes for next command
        if (*end == PIPE)
        {
            *ptrPrevfds = fds;
        }
        //execute the command from remaining commands
        execute_command(line, end, ptrPrevfds);
    }

    return SUCCESS;
}

/******************************************************************************
* The parseCommand function will extracts the first command from commands and  
* removes it from the commands.
******************************************************************************/
int parseCommand(char **ptrline, char **ptrcommand, char *end)
{
    char *line = *ptrline;
    char buff[BUFF_LEN] = {'\0'};
    char *p = buff;
    //trim leading spaces
    while (*line != '\0' && *line == ' ')
        line++;
    //Do not reset the end when there is no line
    if (*line != '\0')
        *end = '\0';
    //break the line at one of the connecting operations
    while (*line != '\0')
    {
        if (*line == PIPE || *line == INPUT || *line == OUTPUT || *line == BKGPROCESS)
        {
            *end = *line;
            line++;
            break;
        }
        else
        {
            *p = *line;
            line++;
            p++;
        }
    }
    //copy the buffer to command pointer
    copybuff(buff, ptrcommand);
    *ptrline = line;
    return strlen(buff);
}

/******************************************************************************
* The tokenize_cmd function will tokenize the command into command and arguments  
* based on spaces between them.
******************************************************************************/
int tokenize_cmd(char *cmd, char **tokens)
{
    char buff[BUFF_LEN] = {'\0'};
    char *p = buff;
    int i = 0;
    //tokenize command by spaces
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
        else if (*cmd == ' ') //trim unnecessary spaces
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
    if (strlen(buff) > 0)
        i++;
    tokens[i] = NULL;
    return i;
}

/******************************************************************************
* The awaitChildren function waits for all the child process to die
******************************************************************************/
short awaitChildren()
{
    pid_t cpid = 0;
    while (1)
    {
        if ((cpid = wait(NULL)) == -1)
        {
            if (errno == ECHILD)
            {
                return SUCCESS;
            }
            else
            {
                perror("wait error");
                return FAILURE;
            }
        }
    }
}