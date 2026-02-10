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

void close_input_buffer(InputBuffer *input_buffer) {
    free(input_buffer->buffer);
    free(input_buffer);
}

void read_input(InputBuffer *input_buffer) {
    ssize_t bytes_read = getline(&(input_buffer->buffer), &(input_buffer->buffer_length), stdin);
    if (bytes_read<0) {
        printf("Reading error");
        exit(EXIT_FAILURE);
    }
    if (input_buffer->buffer[bytes_read - 1] == '\n') {
        input_buffer->input_length = bytes_read - 1;
    }
    input_buffer->buffer[bytes_read - 1] = 0;
}

MetaCommandResult do_meta_command(InputBuffer *input_buffer) {
    if (strcmp(input_buffer->buffer, ".exit") == 0) {
        close_input_buffer(input_buffer);
        exit(EXIT_SUCCESS);
    }

    if (strcmp(input_buffer->buffer, ".help") == 0) {
        printf("\n------- Help Menu -------\n");
        printf(".exit   : Exits the database\n");
        printf(".help   : Prints the help menu\n");
        printf("insert \"username\" \"email\" : Inserts a new user \n");
        printf("select  : Prints database content\n");
        printf("----------------------------\n\n");
        return META_COMMAND_SUCCESS;
    }
    return META_COMMAND_UNRECOGNIZED;
}

PrepareResult prepare_statement(InputBuffer *input_buffer, Statement *statement) {
    if (strncmp(input_buffer->buffer,"insert", 6)==0) {
        statement->statement_type = STATEMENT_INSERT;
        int argc = sscanf(input_buffer->buffer, "insert %s %s", statement->row_to_insert.username, statement->row_to_insert.email);
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

ExecuteResult execute_insert(Statement *statement){
    printf("Insert executed\n");
    return EXECUTE_SUCCESS;
}

ExecuteResult execute_select(Statement *statement){
    printf("Select executed\n");
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
            execute_statement(&input_statement);
        }
    }
    return 0;
}
