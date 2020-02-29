#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char* argv[])
{
    int i = 0;
    char num[3] = "  ";
    for(i; i < 20; i++)
    {
        if(i<10)
        {
            num[0] = '0' + i;
            num[2] = '\0';
        }
        else
        {
            num[0] = '1';
            num[1] = '0' + (i % 10);
            num[2] = ' ';
        }
        write(STDOUT_FILENO, num, 3);
    }
    printf("\n");
    return EXIT_SUCCESS;
}