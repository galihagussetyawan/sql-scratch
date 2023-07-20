#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <ctype.h>

typedef struct
{
    int id;
    char name[32];
    char email[32];
} row_t;

typedef struct
{
    char nama[32];
    char **head;
    row_t *rows;
} table_t;

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
    free(input);
    input = NULL;
}

void parse_meta_command(char *input)
{
    if (strcmp(input, ".exit") == 0)
    {
        close_input(input);
        exit(0);
    }
    else if (strcmp(input, ".clear") == 0)
    {
        system("clear");
    }
    else
    {
        printf("'%s' command is not recognized.\n", input);
    }
}

void parse_statement(char *input, table_t *table)
{
    strlwr(input);

    if (strncmp(input, "insert", 6) == 0)
    {
        printf("Inserting ....\n");

        // harcode insert rows
        table->rows[1].id = 2;
        strcpy(table->rows[1].name, "agusgalih");
        strcpy(table->rows[1].email, "agus@gmail.com");
    }
    else if (strcmp(input, "select") == 0)
    {
        printf("selecting ...\n");

        // hardcode select row
        printf("%d %s %s\n", table->rows[1].id, table->rows[1].name, table->rows[1].email);
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

table_t *init_table()
{
    table_t *table = (table_t *)malloc(sizeof(*table));
    table->head = malloc(3 * sizeof(table->head));
    table->head[0] = malloc(32 * sizeof(char));
    table->head[1] = malloc(32 * sizeof(char));
    table->head[2] = malloc(32 * sizeof(char));
    strcpy(table->nama, "users");
    strcpy(table->head[0], "id");
    strcpy(table->head[1], "name");
    strcpy(table->head[2], "email");

    table->rows = (row_t *)malloc(3 * sizeof(*table->rows));
    table->rows[0].id = 1;
    strcpy(table->rows[0].name, "galihagus");
    strcpy(table->rows[0].email, "galih@gmail.com");

    return table;
}

int main(int argc, char const *argv[])
{
    char *input;
    table_t *table = init_table();
    printf("%d", sizeof(*table->rows) / sizeof(table->rows[0]));

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
                parse_statement(input, table);
            }
        }
    }

    return 0;
}
