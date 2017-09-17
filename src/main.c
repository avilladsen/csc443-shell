#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>

#include "read.h"
#include "write.h"
#include "command.h"
#include "parser.h"

#define INPUT_BUFFER_SIZE 1024

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
    struct Reader *reader = create_reader(STDIN_FILENO);
    
    char *line = read_until(reader, '\n');
    struct Command *command = parse_command(line);
    print_command(command);

    free(line);
    parser_free_command(command);
  }
}
