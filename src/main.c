#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>

#include "command.h"
#include "parser.h"

#define INPUT_BUFFER_SIZE 1024

#define STDIN 0
#define STDOUT 1
#define STDERR 2

void write_out(char *string);

// Read a line from stdin.
char *read_line() {
  char buffer[INPUT_BUFFER_SIZE];

  // TODO handle errors
  size_t length = read(STDIN, buffer, INPUT_BUFFER_SIZE-1);

  // TODO handle multiple lines at once?
  
  char *line = malloc(length + 1);
  memcpy(line, buffer, length);
  line[length]='\0';

  return line;
}

// Write a string.
void write_string(int fd, char *string) {
  size_t length = strlen(string);

  if (length <= 0) {
    return;
  }

  // TODO handle errors
  write(fd, string, length);
}

// Write a string to stdout.
void write_out(char *string) {
  write_string(STDOUT, string);
}

// Write a string to stderror.
void write_error(char *string) {
  write_string(STDERR, string);
}

// Print a Command.
// For debugging purposes.
void print_command(struct Command *command) {
  if (command->input != NULL) {
    write_out("input: ");
    write_out(command->input);
    write_out("\n");
  } else if (command->multiple_inputs) {
    write_out("ERROR: multiple inputs\n");
  }

  if (command->output != NULL) {
    write_out("output: ");
    write_out(command->output);
    if (command->append_output) {
      write_out(" (append)");
    }
    write_out("\n");
  } else if (command->multiple_outputs) {
    write_out("ERROR: multiple outputs\n");
  }



  for (int i = 0; i < command->args_count; i++) {
    write_out("arg: ");
    write_out(command->args[i]);
    write_out("\n");
  }
}


void main() {
  while(true) {
    char *line = read_line();
    struct Command *command = parse_command(line);
    print_command(command);

    free(line);
    parser_free_command(command);
  }
}
