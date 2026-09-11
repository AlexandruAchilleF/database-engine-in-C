#include "functions.h"
#include <stdio.h>
#include <string.h>
#include<stdlib.h>

//Global variables
int global_rows_number = 0;
int max_id = 0;

//Defined functions
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
    if (bytes_read < 0) {
        printf("Reading Error.\n");
        exit(EXIT_FAILURE);
    }
    if (input_buffer->buffer[bytes_read - 1] == '\n') {
        input_buffer->input_length = bytes_read - 1;
        input_buffer->buffer[bytes_read - 1] = 0;
    } else {
        input_buffer->input_length = bytes_read;
    }
}

MetaCommandResult do_meta_command(InputBuffer *input_buffer) {
    if (strcmp(input_buffer->buffer, ".exit") == 0) {
        free_input_buffer(input_buffer);
        exit(EXIT_SUCCESS);
    }
    if (strcmp(input_buffer->buffer, ".help") == 0) {
        printf("\n------- Help Menu -------\n");
        printf(".exit   : Exit the database.\n");
        printf("insert <username> <email> : Insert user.\n");
        printf("select  : Show users.\n");
        printf("delete <id> : Delete user.\n");
        printf("update <id> <username> <email> : Update user.\n");
        printf("-------------------------\n");
        return META_COMMAND_SUCCESS;
    }
    return META_COMMAND_UNRECOGNIZED;
}

PrepareResult prepare_statement(InputBuffer *input_buffer, Statement *statement) {
    if (strncmp(input_buffer->buffer, "insert", 6) == 0) {
        statement->statement_type = STATEMENT_INSERT;
        int argc = sscanf(input_buffer->buffer, "insert %31s %254s",
                          statement->row_to_modify.username, statement->row_to_modify.email);
        if (argc != 2) return PREPARE_SYNTAX_ERROR;
        return PREPARE_SUCCESS;
    }
    if (strncmp(input_buffer->buffer, "select", 6) == 0) {
        statement->statement_type = STATEMENT_SELECT;
        return PREPARE_SUCCESS;
    }
    if (strncmp(input_buffer->buffer, "delete", 6) == 0) {
        statement->statement_type = STATEMENT_DELETE;
        int argc = sscanf(input_buffer->buffer, "delete %d", &statement->row_to_modify.id);
        if (argc != 1) return PREPARE_SYNTAX_ERROR;
        return PREPARE_SUCCESS;
    }
    if (strncmp(input_buffer->buffer, "update", 6) == 0) {
        statement->statement_type = STATEMENT_UPDATE;
        int argc = sscanf(input_buffer->buffer, "update %d %31s %254s",
                          &statement->row_to_modify.id,
                          statement->row_to_modify.username,
                          statement->row_to_modify.email);
        if (argc != 3) return PREPARE_SYNTAX_ERROR;
        return PREPARE_SUCCESS;
    }
    return PREPARE_UNRECOGNIZED_STATEMENT;
}

int find_rows_number(FILE *file) {
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    return (int)(file_size / sizeof(User));
}

int find_max_id(FILE *file) {
    fseek(file, -sizeof(User), SEEK_END);
    User temp_user;
    fread(&temp_user, sizeof(User), 1, file);
    return temp_user.id;
}

ExecuteResult execute_insert(Statement *statement) {
    FILE *file = fopen("file.db", "ab");
    if (!file) return EXECUTE_ERROR;

    statement->row_to_modify.is_deleted = false;
    statement->row_to_modify.id = max_id + 1;
    fwrite(&(statement->row_to_modify), sizeof(User), 1, file);
    fclose(file);

    max_id++;
    global_rows_number++;
    return EXECUTE_SUCCESS;
}

ExecuteResult execute_select(Statement *statement) {
    FILE *file = fopen("file.db", "rb");
    if (!file) return EXECUTE_ERROR;

    User row;
    printf("\n%-4s | %-31s | %s\n", "ID", "Username", "Email");
    printf("--------------------------------------------------------------\n");
    for (int i = 0; i < global_rows_number; i++) {
        fread(&row, sizeof(User), 1, file);
        if (!row.is_deleted) {
            printf("%-4d | %-31s | %s\n", row.id, row.username, row.email);
        }
    }
    printf("--------------------------------------------------------------\n");
    fclose(file);
    return EXECUTE_SUCCESS;
}

ExecuteResult execute_delete(Statement *statement) {
    FILE *file = fopen("file.db", "rb+");
    if (!file) return EXECUTE_ERROR;

    User row;
    for (int i = 0; i < global_rows_number; i++) {
        fread(&row, sizeof(User), 1, file);
        if (row.id == statement->row_to_modify.id) {
            row.is_deleted = true;
            fseek(file, -sizeof(User), SEEK_CUR);
            fwrite(&row, sizeof(User), 1, file);
            fclose(file);
            return EXECUTE_SUCCESS;
        }
    }
    fclose(file);
    return EXECUTE_ERROR;
}

ExecuteResult execute_update(Statement *statement) {
    FILE *file = fopen("file.db", "rb+");
    if (!file) return EXECUTE_ERROR;

    User row;
    for (int i = 0; i < global_rows_number; i++) {
        fread(&row, sizeof(User), 1, file);
        if (row.id == statement->row_to_modify.id) {
            strcpy(row.username, statement->row_to_modify.username);
            strcpy(row.email, statement->row_to_modify.email);

            fseek(file, -sizeof(User), SEEK_CUR);
            fwrite(&row, sizeof(User), 1, file);
            fclose(file);
            return EXECUTE_SUCCESS;
        }
    }
    fclose(file);
    return EXECUTE_ERROR;
}

ExecuteResult execute_statement(Statement *statement) {
    switch (statement->statement_type) {
        case (STATEMENT_INSERT): return execute_insert(statement);
        case (STATEMENT_SELECT): return execute_select(statement);
        case (STATEMENT_DELETE): return execute_delete(statement);
        case (STATEMENT_UPDATE): return execute_update(statement);
        default: return EXECUTE_ERROR;
    }
}
