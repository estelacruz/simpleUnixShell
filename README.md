# Project Overview 

![ee317c5b-3ec4-473f-a7bd-4fa987c846d3-37eb6732dfac small](https://github.com/estelacruz/simpleUnixShell/assets/79770461/629732d8-a08d-4f1b-bab3-7564746037ba)


Designed a simple Unix Shell in C with built-in executable commands such as cd and ls. Supports piping and I/O

# System Design 

# Command Interpretation:
---
The program starts by parsing the user input, splitting it into tokens, and identifying the command name, arguments, and various flags like input and output redirection.

# Error Handling:
---
There are checks for specific commands like "cat" and "echo" to ensure they have at least one argument; otherwise, an error message is printed.

# Path Handling:
---
The code determines whether the command is specified with an absolute path, a relative path, or just the command name.
If it's just the command name, the shell attempts to find the executable in predefined search paths (resembling the $PATH environment variable).
Errors are reported if the executable cannot be found.

# Execution:
---
Based on the path type and the presence of input/output redirection or pipes, the code decides how to execute the command.
It distinguishes between absolute and relative paths for execution. If input/output redirection is detected, it handles those cases accordingly.

# Redirection Handling:
---
The code checks for input and output redirection and distinguishes between overwrite (>) and append (>>) modes.
It ensures that these redirection operations are applied correctly and reports errors if there are multiple redirections or insufficient arguments.

# Loop and Continuation:
---
This system continuously waits for user input and executes commands until exit condition is met.

# Memory Management:
---
The program allocates and frees memory dynamically using malloc and free for certain operations like copying the user command
