# CSC443 Shell Project

This project is Andrew Villadsen's implementation of the shell project for DePaul CSC443.

# Building the shell

Run `make` from the root folder. This will create the shell executable at `bin/shell`.

## No-Prompt mode

Run `make clean noprompt` to build the shell in no-prompt mode. In no-prompt mode, the shell will not print prompts, only piping the output of commands to stdout.

# Running the Shell

Execute `bin/shell` from a terminal.

## Parser Debugger mode

Run `make clean debugparser` to build the shell in the parser debugger mode. Instead of running each command, the shell will print information about how it parsed that command.

# Tests

This project includes a set of functional tests that run through a bash script.

## Running the Tests

While in the `test` directory, run `run_tests.sh` to run all tests for the shell.

## Creating a New Test

While in the `test` directory, run `create_test.sh test_name` to create a new test called "test_name". There will be a new directory under `test` with that name. In that directory, there will be a set of files that describe the test:

* `setup.sh`: The test will run inside a temporary directory. This script runs inside that directory before the test to do any necessary setup.
* `commands.txt`: The input that is piped to the shell.
* `out.txt`: A template that is diffed against the output stream from the shell run. The variable `$TESTROOT` is replaced with the absolute path of the directory the test is run in (in particular, `$TESTROOT$ \n` is the prompt before you move directories.
* `err.txt`: A template that is diffed agains the error stream from the shell run.
* `expect.sh`: A script that is run in the test directory after the shell run. The test fails if it returns an abnormal (non-zero) exit code, in which case it should also output some sort of message to describe the failed expectation.
