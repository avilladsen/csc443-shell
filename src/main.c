#include <stdlib.h>
#include <stdbool.h>

#include "read.h"
#include "write.h"
#include "command.h"
#include "parser.h"
#include "execute.h"

#ifdef DEBUG_PARSER
// Print a Command.
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
#endif /* DEBUG_PARSER */

void prompt() {
#ifndef NO_PROMPT
  char cwd[256];
  if (getcwd(cwd, 256) == NULL) {
    write_error("shell: error getting current working directory\n");
  } else {
    write_out(cwd);
  }
  write_out("$ ");
#endif
}

void main() {
  while(true) {
    prompt();
    struct Reader *reader = create_reader(STDIN_FILENO);
    
    char *line = read_until(reader, '\n');
    struct Command *command = parse_command(line);

#ifdef DEBUG_PARSER
    print_command(command);
#else
    execute_command(command);
#endif

    free(line);
    parser_free_command(command);
  }
}
