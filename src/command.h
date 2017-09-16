#ifndef FILE_COMMAND_INCLUDED
#define FILE_COMMAND_INCLUDED

#include <stdbool.h>

// The results of parsing a command line.
struct Command {
  // Pointer to the allocated command string consumed by parsing.
  // Needed only so that it can be freed properly.
  char *internal_args_ptr;

  // The number of arguments.
  int args_count;
  // The size of the args array.
  int args_size;
  // The array of arguments.
  char** args;

  // The input redirect.
  char* input;
  // Whether there were multiple input redirects.
  bool multiple_inputs;

  // The output redirect.
  char* output;
  // Whether the output should be appended.
  bool append_output;
  // Whether there were multiple output redirects.
  bool multiple_outputs;
};

#endif /* !FILE_COMMAND_INCLUDED */
