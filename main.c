#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <ctype.h>

void parse_meta_command(char *input)
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

void parse_statement(char *input)
{
    strlwr(input);

    if (strcmp(input, "insert") == 0)
    {
        printf("inserting ...\n");
    }
    else if (strcmp(input, "select") == 0)
    {
        printf("selecting ...\n");
    }
    else if (strcmp(input, "delete") == 0)
    {
        printf("deleting ...\n");
    }
    else
    {
        printf("'%s' command is not recognized.\n", input);
    }
}

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
            parse_meta_command(input);
        }
        else
        {
            parse_statement(input);
        }
    }

    return 0;
}
