#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <ctype.h>

#define TABLE_MAX_PAGES 100

volatile int exit_code = 0;

typedef struct
{
    int id;
    char name[32];
    char email[32];
} row_t;

typedef struct
{
    int num_rows;
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

void serialize_table(table_t *table)
{
    FILE *file;
    fopen_s(&file, "data.dat", "w+");
    if (!file)
    {
        perror("failed");
        exit_code = 1;
    }

    fputs("users\n", file);
    for (size_t i = 1; i < table->num_rows + 1; i++)
    {
        fseek(file, ftell(file), SEEK_SET);
        fprintf(file, "%d|%s|%s\n", table->rows[i].id, table->rows[i].name, table->rows[i].email);
    }
    fseek(file, 0, SEEK_END);
    fputs("EOF", file);

    fclose(file);
    free(file);
}

void add_row(row_t *dest, row_t *src)
{
    dest->id = src->id;
    memcpy(dest->name, src->name, 32);
    memcpy(dest->email, src->email, 32);
}

void new_row(table_t *table)
{
    realloc(table->rows, (table->num_rows + 1) * sizeof(row_t));
}

void execute_insert(table_t *table, row_t *row)
{
    if (table->num_rows == 0)
    {
        row->id = 1;
        add_row(&table->rows[table->num_rows], row);
    }
    else
    {
        new_row(table);
        row->id = table->rows[table->num_rows].id + 1;
        add_row(&table->rows[table->num_rows + 1], row);
    }

    table->num_rows += 1;
}

void execute_select(table_t *table, int id)
{
    if (table->num_rows == 0)
    {
        printf("data kosong\n");
    }
    printf("%d ", &table->rows[id].id);
    printf("%s ", table->rows[id].name);
    printf("%s\n", table->rows[id].email);
}

table_t *init_table()
{
    table_t *table = (table_t *)malloc(sizeof(table_t));
    table->rows = (row_t *)malloc(sizeof(row_t));
    table->num_rows = 0;
    return table;
}

void free_table(table_t *table)
{
    free(table);
    table = NULL;
}

void parse_meta_command(char *input, table_t *table)
{
    if (strcmp(input, ".exit") == 0)
    {
        serialize_table(table);
        exit_code = 1;
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
        row_t args;
        sscanf(input, "insert %s %s", &args.name, &args.email);
        execute_insert(table, &args);
    }
    else if (strncmp(input, "select", 6) == 0)
    {
        int id;
        sscanf(input, "select %d", &id);
        execute_select(table, id);
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
    char *input = NULL;
    table_t *table = NULL;
    table = init_table();

    while (!exit_code)
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
                parse_meta_command(input, table);
            }
            else
            {
                parse_statement(input, table);
            }
        }
    }

    close_input(input);
    free_table(table);
    return 0;
}
