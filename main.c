#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <ctype.h>

char *read_input()
{
    char *str, c = '\0';
    int i = 0;

    str = malloc(sizeof(*str));

    while (c != '\n')
    {
        c = fgetc(stdin);
        realloc(str, (i + 1) * sizeof(*str));
        str[i] = c;
        i++;
    }

    str[i - 1] = '\0';
    return str;
}

void close_input(char *input)
{
    input = NULL;
    free(input);
}

void parse_meta_command(char *input)
{
    if (strcmp(input, ".exit") == 0)
    {
        close_input(input);
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
    char *input;

    while (1)
    {
        printf("db > ");
        input = read_input();

        if (strcmp(input, "") == 0)
        {
            printf("'NULL' command is not recognized.\n");
        }
        else
        {
            if (*input == '.')
            {
                parse_meta_command(input);
            }
            else
            {
                parse_statement(input);
            }
        }
    }

    return 0;
}
