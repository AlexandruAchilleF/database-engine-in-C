#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>

#define COLUMN_USERNAME_SIZE 32
#define COLUMN_EMAIL_SIZE 255

typedef struct{
    char *buffer;
    size_t buffer_length;
    ssize_t input_length;
}InputBuffer;

typedef struct {
    int id;
    char username[COLUMN_USERNAME_SIZE];
    char email[COLUMN_EMAIL_SIZE];
}User;

typedef enum {
    META_COMMAND_SUCCESS,
    META_COMMAND_UNRECOGNIZED
} MetaCommandResult;

typedef enum {
    PREPARE_SUCCESS,
    PREPARE_SYNTAX_ERROR,
    PREPARE_UNRECOGNIZED_STATEMENT
}PrepareResult;

typedef enum {
    STATEMENT_INSERT,
    STATEMENT_SELECT
}StatementType;

typedef enum {
    EXECUTE_SUCCESS,
    EXECUTE_ERROR
}ExecuteResult;

typedef struct {
    StatementType statement_type;
    User row_to_insert;//For insert statements only
}Statement;

InputBuffer *new_input_buffer() {
    InputBuffer *input_buffer = malloc(sizeof(InputBuffer));
    input_buffer->buffer = NULL;
    input_buffer->buffer_length = 0;
    input_buffer->input_length = 0;
    return input_buffer;
}

void print_prompt() {
    printf("db > ");
}

void free_input_buffer(InputBuffer *input_buffer) {
    free(input_buffer->buffer);
    free(input_buffer);
}

void read_input(InputBuffer *input_buffer) {
    ssize_t bytes_read = getline(&(input_buffer->buffer), &(input_buffer->buffer_length), stdin);
    if (bytes_read<0) {
        printf("Reading Error.");
        exit(EXIT_FAILURE);
    }
    if (input_buffer->buffer[bytes_read - 1] == '\n') {
        input_buffer->input_length = bytes_read - 1;
    }
    input_buffer->buffer[bytes_read - 1] = 0;
}

MetaCommandResult do_meta_command(InputBuffer *input_buffer) {
    if (strcmp(input_buffer->buffer, ".exit") == 0) {
        free_input_buffer(input_buffer);
        exit(EXIT_SUCCESS);
    }

    if (strcmp(input_buffer->buffer, ".help") == 0) {
        printf("\n------- Help Menu -------\n");
        printf(".exit   : Exits the database.\n");
        printf(".help   : Prints the help menu.\n");
        printf("insert \"username\" \"email\" : Inserts a new user.\n");
        printf("select  : Prints database content.\n");
        printf("----------------------------\n\n");
        return META_COMMAND_SUCCESS;
    }
    return META_COMMAND_UNRECOGNIZED;
}

PrepareResult prepare_statement(InputBuffer *input_buffer, Statement *statement) {
    if (strncmp(input_buffer->buffer,"insert", 6)==0) {
        statement->statement_type = STATEMENT_INSERT;
        int argc =
            sscanf(input_buffer->buffer, "insert %s %s", statement->row_to_insert.username, statement->row_to_insert.email);
        if (argc<2) {
            return PREPARE_SYNTAX_ERROR;
        }
        return PREPARE_SUCCESS;
    }
    if (strncmp(input_buffer->buffer,"select", 6)==0) {
        statement->statement_type = STATEMENT_SELECT;
        return PREPARE_SUCCESS;
    }
    return PREPARE_UNRECOGNIZED_STATEMENT;
}

int global_rows_number;

int find_rows_number(FILE *file){
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    long row_width = sizeof(User);
    int num_rows = file_size / row_width;
    return num_rows;
}

ExecuteResult execute_insert(Statement *statement){
    FILE *file = fopen("file.db", "ab");
    if (file==NULL){
        return EXECUTE_ERROR;
    }
    statement->row_to_insert.id = global_rows_number + 1;
    fwrite(&(statement->row_to_insert), sizeof(User), 1, file);
    fclose(file);
    global_rows_number ++;
    return EXECUTE_SUCCESS;
}

ExecuteResult execute_select(Statement *statement){

    FILE *file = fopen("file.db", "rb");
    if (file==NULL){
        return EXECUTE_ERROR;
    }
    User row;
    printf("\n%-4s | %-31s | %s\n", "ID", "Username", "Email");
    printf("--------------------------------------------------------------\n");
    for (int i = 0; i < global_rows_number; i++) {
        fread(&row, sizeof(User), 1, file);
        printf("%-4d | %-31s | %s\n", row.id, row.username, row.email);
    }
    printf("--------------------------------------------------------------\n");
    fclose(file);
    return EXECUTE_SUCCESS;
}

ExecuteResult execute_statement(Statement *statement) {
    switch (statement->statement_type) {
        case(STATEMENT_INSERT) : {
            return execute_insert(statement);
        }
        case(STATEMENT_SELECT) : {
            return execute_select(statement);
        }
    }
}



int main(int argc, char* argv[]) {
    InputBuffer *input_buffer = new_input_buffer();
    FILE *f = fopen("file.db", "rb");
    if (f == NULL) {
        global_rows_number = 0;
    } else {
        global_rows_number = find_rows_number(f);
        fclose(f);
    }
    fclose(f);
    while (true) {
        print_prompt();
        read_input(input_buffer);
        if (input_buffer->buffer[0] == '.') {
            switch (do_meta_command(input_buffer)) {
                case (META_COMMAND_SUCCESS):
                    continue;
                case (META_COMMAND_UNRECOGNIZED): {
                    printf("Unrecognized command '%s'.\n", input_buffer->buffer);
                    continue;
                }
            }
        }
        else {
            Statement input_statement;
            switch (prepare_statement(input_buffer, &input_statement)) {
                case (PREPARE_SUCCESS) : break;
                case (PREPARE_SYNTAX_ERROR) : {
                    printf("Syntax error: '%s'.\n", input_buffer->buffer);
                    continue;
                }
                    case (PREPARE_UNRECOGNIZED_STATEMENT) : {
                    printf("Unrecognized command: '%s'.\n", input_buffer->buffer);
                    continue;
                }
            }
            switch (execute_statement(&input_statement)) {
                case (EXECUTE_ERROR) : {
                    printf("Command failed\n");
                    break;
                }
                case(EXECUTE_SUCCESS) : {
                    if (input_statement.statement_type==STATEMENT_INSERT)
                        printf("Insert Executed.\n");
                    if (input_statement.statement_type==STATEMENT_SELECT)
                        printf("Select Executed.\n");
                    break;
                }
            }
        }
    }
    return 0;
}
