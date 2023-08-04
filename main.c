#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <ctype.h>

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

void replace(char *str, char find, char change)
{
    int i = 0;
    while (str[i] != '\0')
    {
        if (str[i] == find)
        {
            str[i] = change;
        }
        i++;
    }
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

void deserialize_table(table_t *table)
{
    FILE *file;
    fopen_s(&file, "data.dat", "r");
    if (!file)
    {
        perror("failed");
        exit_code = 1;
    }

    char line[1024];
    int i = 1, i_row = 1;

    while (fgets(line, sizeof(line), file))
    {
        if (strcmp(line, "EOF") == 0)
        {
            break;
        }

        if (i != 1)
        {
            row_t args;

            replace(line, '|', ' ');
            sscanf(line, "%i %s %s", &args.id, &args.name, &args.email);

            new_row(table);
            add_row(&table->rows[i_row], &args);
            table->num_rows = i_row;
            i_row++;
        }

        i++;
    }

    fclose(file);
}

void execute_insert(table_t *table, row_t *row)
{
    new_row(table);
    if (table->num_rows == 0)
    {
        row->id = 1;
        add_row(&table->rows[1], row);
    }
    else
    {
        row->id = table->rows[table->num_rows].id + 1;
        add_row(&table->rows[table->num_rows + 1], row);
    }

    table->num_rows += 1;
}

void execute_select(table_t *table, int idx)
{
    if (table->num_rows == 0)
    {
        printf("data is empty\n");
    }
    else if (idx > table->num_rows)
    {
        printf("index out bound\n");
    }
    else
    {
        printf("%d ", table->rows[idx].id);
        printf("%s ", table->rows[idx].name);
        printf("%s\n", table->rows[idx].email);
    }
}

void execute_delete(table_t *table, int idx)
{
    if (table->num_rows == 0)
    {
        printf("data is empty\n");
    }
    else if (idx > table->num_rows)
    {
        printf("index out bound\n");
    }
    else
    {
        for (int i = idx; i < table->num_rows; i++)
        {
            table->rows[i] = table->rows[i + 1];
        }
        table->num_rows--;
        realloc(table->rows, table->num_rows * sizeof(row_t));
    }
}

table_t *init_table()
{
    table_t *table = (table_t *)malloc(sizeof(table_t));
    table->rows = (row_t *)malloc(table->num_rows * sizeof(row_t));
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
        int idx;
        sscanf(input, "select %d", &idx);
        execute_select(table, idx);
    }
    else if (strncmp(input, "delete", 6) == 0)
    {
        int idx;
        sscanf(input, "delete %i", &idx);
        execute_delete(table, idx);
    }
    else
    {
        printf("'%s' command is not recognized.\n", input);
    }
}

table_t *load_table()
{
    table_t *table = init_table();
    deserialize_table(table);
    return table;
}

int main(int argc, char const *argv[])
{
    char *input;
    table_t *table = load_table();

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