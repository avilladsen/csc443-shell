#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/wait.h>
#include <sys/types.h>

#include "command.h"
#include "write.h"

// Check that the number of arguments meets an expectation.
// If it does not, write a descriptive message to STDERR and return false (0);
// If it does, return true (1);
bool expect_args_count(struct Command *command, int count) {
  if (command->args_count == count) {
    return true;
  } else {
    if (command->args_count > 0) {
      write_error(command->args[0]);
      write_error(": ");
    }
    if (command->args_count < count) {
      write_error("too few arguments\n");
    } else {
      write_error("too many arguments\n");
    }
    return false;
  }
}

// Report an error with the generic error string (i.e. strerror).
// prefixes is NULL, or a NULL-terminated array of strings
// that is written to stderr before the strerror message.
void report_error(int errnum, char** prefixes) {
  if (prefixes != NULL) {
    char **prefix = prefixes;
    while(*prefix != NULL) {
      write_error(*prefix);
      prefix++;
    }
  }
  
  write_error(strerror(errnum));
  write_error("\n");
}

// Execute the cd built-in command.
void change_directory(struct Command *command) {
  if (!expect_args_count(command, 2)) {
    return;
  }

  if (chdir(command->args[1]) < 0) {
    report_error(errno, (char*[]) { "shell: ", command->args[0], ": ", NULL });
  }
}

// Execute the ln command.
void create_link(struct Command *command) {
  if (!expect_args_count(command, 3)) {
    return;
  }

  char **args = command->args;

  if (link(args[1], args[2]) < 0) {
    report_error(errno, (char*[]) { "shell: ", command->args[0], ": ", NULL });
  }
}

// Execute the rm command.
void remove_file(struct Command *command) {
  if (!expect_args_count(command, 2)) {
    return;
  }

  if (unlink(command->args[1]) < 0) {
    write_error("shell: rm: ");
    report_error(errno, (char*[]) { "shell: ", command->args[0], ": ", NULL });
  }
}

// Exit the shell.
void exit_shell(struct Command *command) {
  exit(EXIT_SUCCESS);
}

// Execute a command as a program,
// using the given input and output file descriptors for the executed program.
void execute_program(struct Command *command, int fd_in, int fd_out) {
  pid_t pid = fork();

  if (pid == 0) {
    // in child process

    // apply redirections to stdin and stdout
    if (dup2(fd_in, STDIN_FILENO) < 0) {
      write_error("shell: error performing input redirection\n");
      exit(EXIT_FAILURE);
    }
    if (dup2(fd_out, STDOUT_FILENO) < 0) {
      write_error("shell: error performing output redirection\n");
      exit(EXIT_FAILURE);
    }

    // environment is not handled
    char *envp[] = { NULL };
    execve(command->args[0], command->args, envp);

    // execve failed
    report_error(errno, (char*[]) { "shell: error executing ", command->args[0], ": ", NULL });
    exit(EXIT_FAILURE);
  } else {
    //in shell (parent) process
    
    if (pid < 0) {
      report_error(errno, (char*[]) { "shell: error creating child process: ", NULL });
    }
    
    if (waitpid(pid, NULL, 0) < 0) {
      write_error("shell: error waiting for program to finish execution\n");
    }
  }
}

// Validate a command.
bool validate_command(struct Command *command) {
  if (command->multiple_inputs) {
    write_error("shell: cannot specify more than one input redirection\n");
    return false;
  }

  if (command->multiple_outputs) {
    write_error("shell: cannot specify more than one output redirection\n");
    return false;
  }
  
  if (command->args_count <= 0) {
    if (command->input == NULL && command->output == NULL) {
      // Empty command, so do nothing
    } else {
      write_error("shell: must specify a command for the file redirection\n"); 
    }
    return false;
  }

  return true;
}



// OPEN a file descriptor for the command's input.
// Returns stdin if the command has no input redirection.
// Returns -1 on error.
int open_input(struct Command *command) {
  if (command->input == NULL) {
    return STDIN_FILENO;
  }

  int fd_in = open(command->input, O_RDONLY);

  if (fd_in < 0) {
    report_error(errno, (char*[]) { "shell: error opening input ", command->input, ": ", NULL });
  }

  return fd_in;
}

// Open a file descriptor for the command's output.
// Return stdout if the command has no output redirection.
// Returns -1 on error.
int open_output(struct Command *command) {
  if (command->output == NULL) {
    return STDOUT_FILENO;
  }

  int flags = O_WRONLY | O_CREAT;
  if (command->append_output) {
    flags |= O_APPEND;
  } else {
    flags |= O_TRUNC;
  }

  int fd_out = open(command->output, flags, S_IWUSR | S_IRUSR | S_IRGRP | S_IROTH);

  if (fd_out < 0) {
    report_error(errno, (char*[]) { "shell: error opening output ", command->output, ": ", NULL });
  }

  return fd_out;
}

// Close a file descriptor that was used for input redirection.
void close_input(int fd, char* name) {
  if (fd <= 2) {
    // Don't close stdin, stdout, or stderr
    return;
  }

  if (close(fd) < 0) {
    report_error(errno, (char*[]) { "error closing input ", name, ": ", NULL });
  }
}  

// Close a file descriptor that was used for output redirection.
void close_output(int fd, char* name) {
  if (fd <= 2) {
    // Don't close stdin, stdout, or stderr
    return;
  }

  if (fsync(fd) < 0) {
    report_error(errno, (char*[]) { "error flushing output ", name, ": ", NULL });
  }
  
  if (close(fd) < 0) {
    report_error(errno, (char*[]) { "error closing output ", name, ": ", NULL });
  }
}

// Execute a Command.
void execute_command(struct Command *command) {
   if (!validate_command(command)) {
      return;
  }

  // Start IO redirection.
  // None of the built-in commands do anything with the redirected files,
  // but they DO write empty outputs and fail if the input doesn't exist.
  // Thus, we set up IO redirection here, instead of inside execute_program.

  int fd_in = open_input(command);
  if (fd_in < 0) {
    return;
  }

  int fd_out = open_output(command);
  if (fd_out < 0) {
    return;
  }

  // Delegate command execution to a built-in,
  // or execute a program.
  char *cmd = command->args[0];
  if (strcmp(cmd, "cd") == 0) {
    change_directory(command);
  } else if (strcmp(cmd, "ln") == 0) {
    create_link(command);
  } else if (strcmp(cmd, "rm") == 0) {
    remove_file(command);
  } else if (strcmp(cmd, "exit") == 0) {
    exit_shell(command);
  } else {
    execute_program(command, fd_in, fd_out);
  }

  // Close IO redirection.
  
  close_input(fd_in, command->input);
  close_output(fd_out, command->output);
}
