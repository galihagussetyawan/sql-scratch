#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

int main(int argc, char const *argv[])
{
    char input[128];

    while (1)
    {
        printf("db > ");
        fgets(input, (sizeof(input) - 1), stdin);

        if (*input == '\n')
        {
            printf("'NULL' is not recognized. \n");
        }

        input[strlen(input) - 1] = '\0';

        if (*input == '.')
        {
            if (strcmp(input, ".exit") == 0)
            {
                exit(0);
            }
            else
            {
                printf("'%s' command is not recognized.\n", input);
            }
        }
    }

    return 0;
}
