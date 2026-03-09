#ifndef PARSER_H
#define PARSER_H

#include "errors.h"


// Parses a target string in "file:line" format
// Splits the input into separate file and line components
// @argv: input string in "file:line" format
// @file: output buffer for the file name (modified)
// @line: output buffer for the line number (modified)
// Used by add and remove functions for target parsing
cpin_error_t parser_split_target(char* argv, char** file, char** line);


// Compares the actual argument count against the expected argument count
// @supposed_argc: expected number of arguments for the command
// @argc: actual number of arguments provided
// Returns: 0 on success (counts match), non-zero on failure (counts don't match)
int parser_validate_argc(int supposed_argc, int argc);

// Extracts the node identifier from the argument string
// @argv: input argument string containing node information
char* parser_get_note(char* argv);


#endif
