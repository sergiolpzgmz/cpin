#include "errors.h"

#ifndef FILEIO_H
#define FILEIO_H

typedef struct {
    char* file;      // Path to the file containing the note
    char* line;      // Line number in the file (can be NULL)
    char* content;   // Actual content of the note
} cpin_note_t;

// Creates a new cpin note structure with the specified file, line, and content
// @file: path to the file where the note belongs
// @line: line number in the file (can be NULL)
// @content: the actual note content
// Returns: initialized cpin_note_t structure
cpin_note_t fileio_create_note(char* file, char* line, char* content);

// Frees the memory allocated for a cpin note structure
// @note: pointer to the note structure to free (must not be NULL)
void fileio_note_free(cpin_note_t* note);


// Initializes the hidden .cpin/ subdirectory for cpin configuration
// Creates the necessary directory structure if it doesn't exist
// Returns: CPIN_SUCCESS on success, or appropriate error code from error.h on failure
cpin_error_t fileio_initialize(cpin_note_t* node);

// Saves an individual node to the specified file and line location
// @file: path to the file where the node should be saved
// @line: line number in the file (can be NULL for default positioning)
// @node: node identifier to save
// Returns: CPIN_SUCCESS on success, or appropriate error code from error.h on failure
cpin_error_t fileio_save(cpin_note_t* node);

// Loads nodes from a specified file and optionally line
// @file: path to the file to load from
// @line: specific line number to load from (can be NULL for entire file)
// @result: pointer to store the loaded results (must be pre-allocated)
// Returns: CPIN_SUCCESS on success, or appropriate error code from error.h on failure
cpin_error_t fileio_load(char* file, char* line, char** result);

// Deletes a specified node from the file:line format
// @file: path to the file containing the node
// @line: line number where the note is located
// Returns: CPIN_SUCCESS on success, or appropriate error code from error.h on failure
cpin_error_t fileio_delete(char* file, char* line);

#endif
