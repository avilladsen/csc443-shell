# CSC443 Shell Project

This project is Andrew Villadsen's implementation of the shell project for DePaul CSC443.

# Building the shell

Run `make` from the root folder.

## No-Prompt mode

Run `make clean noprompt` to build the shell in no-prompt mode. In no-prompt mode, the shell will not print prompts, only piping the output of commands to stdout.

## Parser Debugger mode

Run `make clean debugparser` to build the shell in the parser debugger mode. Instead of running each command, the shell will print information about how it parsed that command.

# Running Tests

While in the `test` directory, run `run_tests.sh` to run all tests for the shell.