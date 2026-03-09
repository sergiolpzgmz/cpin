#include "errors.h"

const char* error_to_string(cpin_error_t error) {
    switch (error) {
        case CPIN_SUCCESS:              return "success";
        case CPIN_ERR_INVALID_ARGS:    return "invalid arguments";
        case CPIN_ERR_INVALID_TARGET:  return "invalid target (expected file:line)";
        case CPIN_ERR_FILE_NOT_FOUND:  return "file not found";
        case CPIN_ERR_NO_LINE_SPECIFIED: return "no line specified (expected file:line)";
        case CPIN_ERR_STORAGE_INIT:    return "failed to initialize .cpin storage";
        case CPIN_ERR_NOTE_NOT_FOUND:  return "note not found";
        case CPIN_ERR_WRITE_FAILED:    return "write failed";
        default:                       return "unknown error";
    }
}
