#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>

#define INPUT_BUFFER_SIZE 1024

char *read_line() {
  char buffer[INPUT_BUFFER_SIZE];

  // TODO handle errors
  size_t length = read(0, buffer, INPUT_BUFFER_SIZE);

  // TODO handle multiple lines at once?
  
  char *line = malloc(length + 1);
  memcpy(line, buffer, length);

  return line;
}

void write_string(int fd, char *string) {
  size_t length = strlen(string);

  // TODO handle errors
  write(1, string, length);
}

void main() {
  while(true) {
    char *line = read_line();
    write_string(1, line);
  }
}
