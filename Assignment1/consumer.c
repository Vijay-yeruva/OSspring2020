#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
    int i = 0;
    char num[3] = {'\0'};
    while(read(STDIN_FILENO, num, 3) > 0)
    {
        int i = atoi(num);
        if(i % 2 == 0)
        {
            printf("%d ", i);
        } 
    }
    printf("\n");
    return EXIT_SUCCESS;
}