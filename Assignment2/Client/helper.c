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

/********************************
 * This function parsed the request
 *********************************/
int parseRequest(char* request, char** command, char* ch, char** filename)
{
    char* cmd = *command;
    char* file = *filename;
    **command = '\0';
    *ch = '\0';
    **filename = '\0';

    while(request != NULL && *request != '\0')
    {
        if(*request != ' ')
        {
            *cmd = *request;
            cmd++;
            request++;
        } 
        else
        {
            break;
        }
    }
    cmd++;
    *cmd = '\0';
    if (strcmp(*command, CMD_TOUPPER) == SUCCESS)
    {
        //Need to read only the file name
        while(request != NULL && *request != '\0')
        {
            if(*request != ' ' && *request != '<' && *request != '>')
            {
                *file = *request;
                file++;
                request++;
            } 
            else
            {
                request++;
                continue;
            }
        }
    }
    else if (strcmp(*command, CMD_COUNT) == SUCCESS)
    {
        //Need to read char 
        while(request != NULL && *request != '\0')
        {
            if(*request != ' ' && *request != '<' && *request != '>' && *request != ',')
            {
                *ch = *request;
                request++;
            }
            else if(*request == ',')
            {
                request++;
                break;
            } 
            else
            {
                request++;
                continue;
            }
        }
        //file name
        while(request != NULL && *request != '\0')
        {
            if(*request != ' ' && *request != '<' && *request != '>')
            {
                *file = *request;
                file++;
                request++;
            } 
            else
            {
                request++;
                continue;
            }
        }   
    }
    else
    {
        return FAILURE;
    }
    
    return SUCCESS;
}
