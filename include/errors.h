// errors.h
#ifndef ERRORS_H
#define ERRORS_H

typedef enum {
    CPIN_SUCCESS = 0,           // Operation completed successfully
    CPIN_ERR_INVALID_ARGS,      // Invalid function arguments provided
    CPIN_ERR_INVALID_TARGET,    // Target format is invalid (expected file:line)
    CPIN_ERR_FILE_NOT_FOUND,    // Specified file could not be found
    CPIN_ERR_NO_LINE_SPECIFIED,
    CPIN_ERR_STORAGE_INIT,      // Failed to initialize storage directory
    CPIN_ERR_NOTE_NOT_FOUND,    // Specified node/entry not found
    CPIN_ERR_WRITE_FAILED,      // Failed to write to file or storage
} cpin_error_t;

// Converts an error code to a human-readable string representation
// @error: the error code to convert
// Returns: pointer to static string describing the error
const char* error_to_string(cpin_error_t error);

#endif
