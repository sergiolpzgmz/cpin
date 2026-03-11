#include "errors.h"

#ifndef FILEIO_H
#define FILEIO_H

typedef struct {
    char* file;      // Path to the file containing the note
    char* line;      // Line number in the file (can be NULL)
    char* content;   // Actual content of the note
} cpin_note_t;

// Checks if the error code indicates 'No such file or directory'
// @e: the errno value to check
// Returns: true if the error is ENOENT, false otherwise
#define FILE_NOT_FOUND(e) ((e) == ENOENT)

// Creates a new cpin note structure with the specified file, line, and content
// @file: path to the file where the note belongs
// @line: line number in the file (can be NULL)
// @content: the actual note content
// Returns: initialized cpin_note_t structure
cpin_note_t fileio_create_note(char* file, char* line, char* content);

// Frees the memory allocated for a cpin note structure
// @note: pointer to the note structure to free (must not be NULL)
void fileio_note_free(cpin_note_t* note);

// Saves a note to the given notes file path
// @node: the note to save
// @notes_path: path to the notes file (local or global)
// Returns: CPIN_SUCCESS on success, or appropriate error code on failure
cpin_error_t fileio_save(cpin_note_t* node, const char* notes_path);

// Loads notes matching file (and optionally line) into a newly allocated string (*result).
// @file: path to the file to filter by
// @line: specific line number to filter by (can be NULL for entire file)
// @notes_path: path to the notes file (local or global)
// @result: pointer to store the loaded results — caller must free(*result)
// Returns: CPIN_SUCCESS on success, or appropriate error code on failure
cpin_error_t fileio_load(char* file, char* line, const char* notes_path, char** result);

// Loads all notes from the notes file into a newly allocated string (*result).
// @notes_path: path to the notes file (local or global)
// @result: pointer to store the loaded results — caller must free(*result)
// Returns: CPIN_SUCCESS on success, CPIN_ERR_NOTE_NOT_FOUND if no notes exist
cpin_error_t fileio_load_all(const char* notes_path, char** result);

// Deletes all notes matching file:line from the notes file (rewrites the file).
// @file: path to the file containing the note
// @line: line number where the note is located
// @notes_path: path to the notes file (local or global)
// Returns: CPIN_SUCCESS on success, or appropriate error code on failure
cpin_error_t fileio_delete(char* file, char* line, const char* notes_path);

// Searches all notes for lines whose content contains the given keyword.
// @keyword: substring to search for in note content
// @notes_path: path to the notes file (local or global)
// @result: pointer to store the matching results — caller must free(*result)
// Returns: CPIN_SUCCESS on success, CPIN_ERR_NOTE_NOT_FOUND if no matches
cpin_error_t fileio_search(const char* keyword, const char* notes_path, char** result);

// Check if target file exist on disk
// @file: path to the file to be checked on the disk
// Returns: CPIN_SUCCESS on success, CPIN_ERR_FILE_NOT_EXIST if no file exist
cpin_error_t fileio_file_exist(char* file);

#endif
