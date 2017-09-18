
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "read.h"

#define INITIAL_BUFFER_SIZE 1024
#define GROW_BUFFER_SIZE(size) (2 * size)
#define MAX_READ_SIZE 1024

// Create a new Reader from a file descriptor.
struct Reader *create_reader(int fd) {
  struct Reader *reader = malloc(sizeof(struct Reader));

  reader->fd = fd;

  reader->buffer = malloc(INITIAL_BUFFER_SIZE);
  reader->buffer_size = INITIAL_BUFFER_SIZE;

  reader->start = 0;
  reader->count = 0;
  reader->read_eof = false;
}

// Free a Reader.
void free_reader(struct Reader *reader) {
  free(reader->buffer);
  free(reader);
}

// Grow the buffer for the reader.
void grow_buffer(struct Reader *reader) {
  ssize_t copy_count = reader->count - (reader->buffer_size - reader->start);

  size_t old_buffer_size = reader->buffer_size;
  reader->buffer_size = GROW_BUFFER_SIZE(old_buffer_size);
  reader->buffer = realloc(reader->buffer, reader->buffer_size);

  if (copy_count > 0) {
    memcpy(reader->buffer, reader->buffer + old_buffer_size, copy_count);
  }
}

// Read more into the buffer.
void read_more(struct Reader *reader) {
  if (reader->read_eof) {
    return;
  }
  
  size_t read_start = reader->start + reader->count;
  ssize_t read_count = reader->buffer_size - read_start;

  if (read_count <= 0) {
    // We've wrapped back to the start of the buffer.
    read_start = -read_count;
    read_count = reader->start - (reader->count - reader->buffer_size);
  }

  if (read_count < 0) {
    // This should never happen. Return to avoid buffer overflow.
    return;
  }

  if (read_count == 0) {
    // The buffer is full.
    grow_buffer(reader);
    read_more(reader);
    return;
  }

  if (read_count > MAX_READ_SIZE) {
    read_count = MAX_READ_SIZE;
  }
    

  ssize_t bytes_read = read(reader->fd, reader->buffer + read_start, read_count);

  if (bytes_read < 0) {
    // TODO error handling
  }

  if (bytes_read == 0) {
    reader->read_eof = true;
  }

  reader->count += bytes_read;
}

// Read a string of the given length from the Reader.
// Will return a string of shorter length if EOF is encountered before reading that length.
char *read_string(struct Reader *reader, size_t length) {
  while(!reader->read_eof && length > reader->count) {
    read_more(reader);
  }

  if (reader->count < length) {
    // Caller is asking for past EOF, so give until EOF.
    length = reader->count;
  }

  char *string = malloc(length + 1);

  if (reader->start + length <= reader->buffer_size) {
    memcpy(string, reader->buffer + reader->start, length);
  } else {
    int count_before_buffer_end = reader->buffer_size - reader->start;
    memcpy(string, reader->buffer + reader->start, count_before_buffer_end);
    memcpy(string, reader->buffer, length - count_before_buffer_end);
  }
  string[length] = '\0';

  reader->count -= length;
  reader->start = (reader->start + length % reader->buffer_size);

  return string;
}

// Read from the Reader up to and including a terminator character, or until EOF.
// Returns a string containing what was read.
char *read_until(struct Reader *reader, char terminator) {
  size_t length = 0;

  char *buffer = reader->buffer;
  size_t start = reader->start;
  size_t buffer_size = reader->buffer_size;

  // Keep searching while we haven't searched past the EOF,
  // and the current character isn't the terminator.
  while (!(reader->read_eof && length >= reader->count) && \
	 (length < 1 || buffer[(start + length - 1) % buffer_size] != terminator)) {
    length++;
    if (length > reader->count) {
      read_more(reader);
      buffer = reader->buffer;
      start = reader->start;
      buffer_size = reader->buffer_size;
    }
  }

  return read_string(reader, length);
}
