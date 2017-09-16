#ifndef FILE_PARSER_INCLUDED
#define FILE_PARSER_INCLUDED

#include "command.h"

// Parse a command line string.
struct Command *parse_command(char* command_line);

// Free a Command structure created by the parser.
void parser_free_command(struct Command *result);

#endif /* !FILE_PARSER_INCLUDED */
