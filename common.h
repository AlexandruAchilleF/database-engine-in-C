#ifndef COMMON_H
#define COMMON_H

//Used libraries
#include <stddef.h>
#include <sys/types.h>
#include <stdbool.h>

//Defined constants
#define COLUMN_USERNAME_SIZE 32
#define COLUMN_EMAIL_SIZE 255

//Defined enumerations
typedef enum {
    META_COMMAND_SUCCESS,
    META_COMMAND_UNRECOGNIZED
} MetaCommandResult;

typedef enum {
    PREPARE_SUCCESS,
    PREPARE_SYNTAX_ERROR,
    PREPARE_UNRECOGNIZED_STATEMENT
} PrepareResult;

typedef enum {
    STATEMENT_INSERT,
    STATEMENT_SELECT,
    STATEMENT_DELETE,
    STATEMENT_UPDATE
} StatementType;

typedef enum {
    EXECUTE_SUCCESS,
    EXECUTE_ERROR
} ExecuteResult;

//Defined structures
typedef struct {
    int id;
    char username[COLUMN_USERNAME_SIZE];
    char email[COLUMN_EMAIL_SIZE];
    bool is_deleted;
} User;

typedef struct {
    StatementType statement_type;
    User row_to_modify; //Used for insert/update/delete statements
} Statement;

typedef struct {
    char *buffer;
    size_t buffer_length;
    ssize_t input_length;
} InputBuffer;

//Extern globals
extern int global_rows_number;
extern int max_id;

#endif // COMMON_H