#ifndef FILE_READ_INCLUDED
#define FILE_READ_INCLUDED

#include <unistd.h>
#include <stdbool.h>

struct Reader {
  int fd;
  void *buffer;
  size_t buffer_size;

  size_t start;
  size_t count;
  bool read_eof;
};

// Create a new Reader from a file descriptor.
struct Reader *create_reader(int fd);

// Free a Reader.
void free_reader(struct Reader *reader);

// Read a string of the given length from the Reader.
// Will return a string of shorter length if EOF is encountered before reading that length.
char *read_string(struct Reader *reader, size_t length);

// Read from the Reader up to and including a terminator character, or until EOF.
// Returns a string containing what was read.
char *read_until(struct Reader *reader, char terminator);

#endif /* !FILE_READ_INCLUDED */
