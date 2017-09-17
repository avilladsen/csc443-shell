#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>

#include "command.h"
#include "parser.h"

#define INITIAL_ARGS_SIZE 1
#define GROW_ARGS_SIZE(size) (2 * size)

// Create an (unprocessed) Command for a command.
struct Command *create_parser_result(char *command_line) {
  struct Command *command = malloc(sizeof(struct Command));

  command->internal_args_ptr = malloc((strlen(command_line)+1)*sizeof(char));
  strcpy(command->internal_args_ptr, command_line);

  // calloc to initialize array with NULLs
  command->args = calloc(INITIAL_ARGS_SIZE,  sizeof(char*));
  command->args_size = INITIAL_ARGS_SIZE;
  command->args_count = 0;

  command->input = NULL;
  command->multiple_inputs = false;

  command->output = NULL;
  command->append_output = false;
  command->multiple_outputs = false;

  return command;
}

// Free a Command structure.
void parser_free_command(struct Command *command) {
  free(command->args);
  free(command->internal_args_ptr);
  free(command);
}

// Add an argument to the end of this command's arguments array.
void add_arg(struct Command *command, char* arg) {
  int arg_idx = command->args_count;

  // Check if there's only one position left,
  // since we always want to keep a trailing NULL for the sake of execve()
  if (arg_idx >= command->args_size - 1) {
    int old_args_size = command->args_size;
    command->args_size = GROW_ARGS_SIZE(old_args_size);
    command->args = realloc(command->args, command->args_size * sizeof(char*));
    // null out the new positions to make sure the array stays NULL terminated
    for (int i = old_args_size; i < command->args_size; i++) {
      command->args[i] = NULL;
    }
  }

  command->args_count = arg_idx + 1;
  command->args[arg_idx] = arg;
}

// Add an input redirect to the command.
// Nulls the input and sets multiple_inputs if an input already exists.
void add_input(struct Command *command, char* input) {
  if (command->multiple_inputs) {
    return;
  }

  if (command->input == NULL) {
    command->input = input;
  } else {
    command->input = NULL;
    command->multiple_inputs = true;
  }
}

// Add an output redirect to the command.
// Nulls the output and sets multiple_outputs if an output already exists.
void add_output(struct Command *command, char* output, bool append) {
  if (command->multiple_outputs) {
    return;
  }

  if (command->output == NULL) {
    command->output = output;
    command->append_output = append;
  } else {
    command->output == NULL;
    command->append_output = false;
    command->multiple_outputs = true;
  }
}

// Consume leading whitespace, replacing it with '\0'.
// Returns a pointer to the first non-whitespace character in the string,
// or to the terminating '\0' if there were none.
char *consume_whitespace(char *string) {
  char *ptr = string;
  while(isspace(*ptr)) {
    *ptr = '\0';
    ptr++;
  }
  return ptr;
}

// Moves over leading non-whitespace, non-redirect characters, then consumes whitespace.
// Returns a pointer to the first non-whitespace character after consuming whitespace,
// or to the terminating '\0' if there were none.
char *next_token(char *string) {
  char *next = string;
  while(!(isspace(*next) || *next == '\0' || *next == '<' || *next == '>')) {
    next++;
  }
  return consume_whitespace(next);
}

struct Command *parse_command(char* command_line) {
  struct Command *command = create_parser_result(command_line);

  char *curr_idx = command->internal_args_ptr;

  curr_idx = consume_whitespace(curr_idx);
  while(*curr_idx != '\0') {
    switch(*curr_idx) {
      case '<':
	*curr_idx = '\0';
	curr_idx++;
	curr_idx = consume_whitespace(curr_idx);
        add_input(command, curr_idx);
        break;
      case '>':
	*curr_idx = '\0';
	curr_idx++;
	bool append = false;
	if (*curr_idx == '>') {
	  append = true;
	  curr_idx++;
	}
        curr_idx = consume_whitespace(curr_idx);
	add_output(command, curr_idx, append);
        break;
      default:
	add_arg(command, curr_idx);
    }
    curr_idx = next_token(curr_idx);
  }

  return command;
}
