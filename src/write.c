#include <unistd.h>
#include <stdlib.h>
#include <string.h>

// Write a string.
// Returns -1 on an error, and 0 on success.
int write_string(int fd, char *string) {
  size_t length = strlen(string);

  if (length <= 0) {
    // No need to write an empty string
    return 0;
  }

  ssize_t written = 0;
  int no_write_count = 0;

  while (written < length) {
    ssize_t new_written = write(fd, string + written, length - written);

    if (new_written < 0) {
      // Pass through an error.
      return -1;
    }

    if (new_written > 0) {
      no_write_count = 0;
    } else {
      no_write_count++;
    }

    if (no_write_count >= 3) {
      // Fail after a few attempts to write that don't progress,
      // since we don't want to get stuck forever.
      return -1;
    }

    written += new_written;
  }

  return 0;
}

// Write a string to stdout.
void write_out(char *string) {
  if (write_string(STDOUT_FILENO, string) < 0) {
    write_string(STDERR_FILENO, "Error writing to STDIN");
    // If we're having trouble writing to STDIN, something is serverely wrong: let's give up!
    exit(EXIT_FAILURE);
  }
}

// Write a string to stderror.
void write_error(char *string) {
  if (write_string(STDERR_FILENO, string) < 0) {
    // Bit of a longshot for this write to work, but hey
    write_string(STDERR_FILENO, "Error writing to STDERR");
    // If we're having trouble writing to STERR, something is serverely wrong: let's give up!
    exit(EXIT_FAILURE);
  }
}
