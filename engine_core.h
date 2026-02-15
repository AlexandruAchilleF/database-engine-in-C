#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <stdio.h>
#include "common.h"

InputBuffer *new_input_buffer();
void print_prompt();
void free_input_buffer(InputBuffer *input_buffer);
void read_input(InputBuffer *input_buffer);
MetaCommandResult do_meta_command(InputBuffer *input_buffer);
PrepareResult prepare_statement(InputBuffer *input_buffer, Statement *statement);
int find_rows_number(FILE *file);
int find_max_id(FILE *file);
ExecuteResult execute_statement(Statement *statement);

#endif//FUNCTIONS_H
