# Overview

This is Andrew Villadsen's submission for the shell project of CSC443. See Readme.md in the project repo for details on build or running the shell executable (but the punchline is `make && bin/shell`).

# Assumptions

One thing that I haven't had the time to look into is the recoverability of errors in reading or writing to stdin, stdout, or stderr. I decided to assume that such errors are unrecoverable, and to just make a best effort to inform the user before exiting.

# Design Decisions

I factored the shell into five core pieces of functionality: reading input (reader.h), writing output and errors (writer.h), parsing the command line (parser.h), executing the parsed command (execute.h), and finally the REPL loop itself (main.c). I also separated out the contract between the parser and executor (command.h), so that neither would have to depend on the other.

I originally wanted to isolate the I/O operations to the REPL to make it easier to unit test the components of the shell. However, I quickly realized that it would be difficult to isolate the executor, where pretty much all errors happen, from writing errors. If I were to continue with this project, rather than isolating the executor from writing I would create a writer object that abstracts over writes to out and error, sending them to chosen file descriptors rather than stdout and stderr. Not only would this enable unit testing, but it would also be a good step towards features like command piping and error redirection.

## Design for Tests

Faced with limited time, I abandoned my plans for unit tests in favor of focusing on functional tests, since I felt they were more valuable for this small project. Going over a few tests cases (notably for cd and rm), I identified a common Given-When-Then pattern that I could use for tests:

* Given that the shell starts in a directory with certain contents,
* When a series of commands are executed in the shell (i.e. are piped to stdin),
* Then certain content will be piped to stdin and stdout, and the directory will have contents of some particular form.

To support this pattern, I wrote a simple shell script that for each test creates a temporary directory, sets up that directory for the test (setup.sh), runs shell and pipes in a set of commands (commands.txt), then diffs the output and error streams against templates (out.txt. and err.txt), and then finally checks the state of the directory (expect.sh). In retrospect, it would have been simpler to include "before" and "after" directories with each test, and do setup with a copy and the expectation with a folder diff.

# Summary & Lessons Learned

This is my first time writing code in C in 7 years, so most of my lessons learned on this project have just been about how to write C again. I had some harsh reminders along the way that small errors are much more dire in a language without built-in array-bounds checking. And a lot of the tools I've come to rely on for abstraction, encapsulation, and soundness proving (e.g. interfaces and generics, private members, type systems) are non-existant or severely limited in C. In addition to getting into the guts of the operating system, I'm looking forward to learning how to write good, defensive code in C.

# Appendix

Since this version of the report is included inside the repo, I an omitting the code appendix. See the `src` folder for the source files.