#include <string.h>
#include "errors.h"
#include "parser.h"

// Splits "file:line" into two pointers into the original string (mutates argv).
// Content after the second colon is ignored here.
cpin_error_t parser_split_target(char* argv, char** file, char** line) {
    if (!argv || !file || !line) return CPIN_ERR_INVALID_ARGS;

    char* colon = strchr(argv, ':');
    if (!colon) return CPIN_ERR_INVALID_TARGET;

    *colon = '\0';       // terminate file portion in-place
    *file = argv;
    *line = colon + 1;   // points to line number string

    if (**line == '\0') return CPIN_ERR_NO_LINE_SPECIFIED;

    return CPIN_SUCCESS;
}

// Returns 0 (success) when argc matches the expected count, 1 otherwise.
int parser_validate_argc(int supposed_argc, int argc) {
    return (argc == supposed_argc) ? 0 : 1;
}

// The note content is passed verbatim as argv — just return it.
char* parser_get_note(char* argv) {
    return argv;
}
