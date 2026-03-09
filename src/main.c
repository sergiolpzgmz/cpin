#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "errors.h"
#include "parser.h"
#include "fileio.h"

static void usage(void) {
    printf("Usage:\n");
    printf("  cpin add <file:line> \"<note>\"\n");
    printf("  cpin list [file] [line]\n");
    printf("  cpin remove <file:line>\n");
}

int main(int argc, char** argv) {
    if (argc < 2) {
        usage();
        return 1;
    }

    char* cmd = argv[1];

    // ── add ───────────────────────────────────────────────────────────────────
    if (!strcmp(cmd, "add")) {
        if (argc < 4) {
            printf("Usage: cpin add <file:line> \"<note>\"\n");
            return 1;
        }

        char* file = NULL;
        char* line = NULL;
        cpin_error_t err = parser_split_target(argv[2], &file, &line);
        if (err != CPIN_SUCCESS) {
            printf("Error: %s\n", error_to_string(err));
            return 1;
        }

        char* content = parser_get_note(argv[3]);
        if (!content || content[0] == '\0') {
            printf("Error: note content cannot be empty\n");
            return 1;
        }

        cpin_note_t note = fileio_create_note(file, line, content);
        err = fileio_save(&note);
        fileio_note_free(&note);

        if (err != CPIN_SUCCESS) {
            printf("Error: %s\n", error_to_string(err));
            return 1;
        }

        printf("Note added: %s:%s\n", file, line);
        return 0;
    }

    // ── list ──────────────────────────────────────────────────────────────────
    if (!strcmp(cmd, "list")) {
        char* result = NULL;
        cpin_error_t err;

        if (argc < 3) {
            err = fileio_load_all(&result);
            if (err == CPIN_ERR_NOTE_NOT_FOUND || !result) {
                printf("No notes found in this project\n");
                return 0;
            }
        } else {
            char* file = argv[2];
            char* line = (argc >= 4) ? argv[3] : NULL;
            err = fileio_load(file, line, &result);
            if (err == CPIN_ERR_NOTE_NOT_FOUND || !result) {
                printf("No notes found for %s%s%s\n",
                       file, line ? ":" : "", line ? line : "");
                return 0;
            }
        }

        if (err != CPIN_SUCCESS) {
            printf("Error: %s\n", error_to_string(err));
            return 1;
        }

        printf("%s", result);
        free(result);
        return 0;
    }

    // ── remove ────────────────────────────────────────────────────────────────
    if (!strcmp(cmd, "remove")) {
        if (argc < 3) {
            printf("Usage: cpin remove <file:line>\n");
            return 1;
        }

        char* file = NULL;
        char* line = NULL;
        cpin_error_t err = parser_split_target(argv[2], &file, &line);
        if (err != CPIN_SUCCESS) {
            printf("Error: %s\n", error_to_string(err));
            return 1;
        }

        err = fileio_delete(file, line);
        if (err != CPIN_SUCCESS) {
            printf("Error: %s\n", error_to_string(err));
            return 1;
        }

        printf("Note removed: %s:%s\n", file, line);
        return 0;
    }

    printf("Unknown command: %s\n", cmd);
    usage();
    return 1;
}
