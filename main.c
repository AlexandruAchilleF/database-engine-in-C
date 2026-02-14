#include <stdio.h>
#include "functions.h"

int main(int argc, char* argv[]) {
    InputBuffer *input_buffer = new_input_buffer();

    FILE *f = fopen("file.db", "rb");
    if (f == NULL) {
        global_rows_number = 0;
        max_id = 0;
    } else {
        global_rows_number = find_rows_number(f);
        if (global_rows_number > 0) {
            max_id = find_max_id(f);
        } else {
            max_id = 0;
        }
        fclose(f);
    }

    while (true) {
        print_prompt();
        read_input(input_buffer);

        if (input_buffer->buffer[0] == '.') {
            switch (do_meta_command(input_buffer)) {
                case (META_COMMAND_SUCCESS): continue;
                case (META_COMMAND_UNRECOGNIZED):
                    printf("Unrecognized command: '%s'.\n", input_buffer->buffer);
                    continue;
            }
        }

        Statement input_statement;
        switch (prepare_statement(input_buffer, &input_statement)) {
            case (PREPARE_SUCCESS): break;
            case (PREPARE_SYNTAX_ERROR):
                printf("Syntax error.\n");
                continue;
            case (PREPARE_UNRECOGNIZED_STATEMENT):
                printf("Unrecognized keyword.\n");
                continue;
        }

        switch (execute_statement(&input_statement)) {
            case (EXECUTE_SUCCESS):
                printf("Executed.\n");
                break;
            case (EXECUTE_ERROR):
                printf("Error: Operation failed (ID not found or DB error).\n");
                break;
        }
    }
    return 0;
}