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
    char* command;
    char end = 0;
    printf("%s", "parsing commands\n");
    int len = parseCommand(line, command, &end);
    printf("%s\n", "parsed commands");
    if(len > 0)
    printf("%s %s %d %s", "executing command: ",command,len, "\n");
    status = execute_command(command, &end);
    printf("%s", "executed commands\n");
    return status;
}

int parseCommand(char *line, char *command, char* end)
{
    char buff[BUFF_LEN] = {'\0'};
    char *p = buff;
    printf("parsing line: %s\n",line);
    while (*line != '\0')
    {
        if (*line == '|' || *line == '<' || *line == '>' || *line == '&')
        {
            *end = *line;
            break;
        }
        else
        {
            *p = *line;
            printf("char: %c, %c, %p, %p\n",*line, *p, line, p);
            line++;
            p++;
        }
    }
    copybuff(buff, command);
    printf("%p parsed command: %s\n", command, command);
    return strlen(buff);
}

short execute_command(char* cmd, char* end)
{
    short status = SUCCESS;
    int result;
    pid_t child_pid;
    char* tokens[MAX_TOKEN];
    printf("%s", "tokenizing commands\n");
    int i = 0, tcount = tokenize_cmd(cmd, tokens);

    // Fork the child process
    // child_pid = fork();

    // Check for errors in fork()
    switch (child_pid) {
    case EAGAIN:
        //Error due to I/O
        perror("Error EAGAIN");
        return EAGAIN;
    case ENOMEM:
        //Not able to allocate memory error
        perror("Error ENOMEM");
        return ENOMEM;
    case ENOSYS:
        //fork is not suppoted on this system
        perror("Error ENOSYS");
        return ENOSYS;
    }
    
    for(i = 0; i < tcount; i++)
    {
        printf("%s\n", tokens[i]);
    }
    
    if (child_pid == 0) {

        // Set up redirection in the child process
        // if (i)
        //     //if there is an input symbol
        //     freopen(i_name, "r", stdin);

        // if (o)
        //     //if there is an output symbol
        //     freopen(o_name, "w+", stdout);

        // // Execute the command
        // result = execvp(tokens[0], tokens);

        exit(-1);
    }

    return status;
}

int tokenize_cmd(char* cmd, char** tokens)
{
    printf("%s\n", cmd);
    char buff[BUFF_LEN] = {'\0'};
    char *p = buff;
    int i = 0;
    while (*cmd != '\0')
    {
        printf("%s\n", cmd);
        if (*cmd == ' ' && *(cmd+1) != ' ')
        {
            copybuff(buff, tokens[i]);
            memset(buff, '\0', BUFF_LEN);
            p = buff;
            i++;
            cmd++;
        }
        else if(*cmd == ' ')
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
    return i;
}