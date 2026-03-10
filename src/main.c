#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "errors.h"
#include "parser.h"
#include "fileio.h"

static void usage(void) {
    printf("Usage:\n");
    printf("  cpin add <file:line> \"<note>\" [--global]\n");
    printf("  cpin list [file] [line] [--global]\n");
    printf("  cpin remove <file:line> [--global]\n");
    printf("  cpin search <keyword> [--global]\n");
}

static const char* resolve_notes_path(int global) {
    if (!global) return ".cpin/notes";

    static char path[4096];
    const char* home = getenv("HOME");
    if (!home) {
        fprintf(stderr, "Error: $HOME is not set\n");
        exit(1);
    }
    snprintf(path, sizeof(path), "%s/.cpin/notes", home);
    return path;
}

int main(int argc, char** argv) {
    // Strip --global flag from argv before command dispatch
    int global = 0;
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--global") == 0) {
            global = 1;
            for (int j = i; j < argc - 1; j++)
                argv[j] = argv[j + 1];
            argc--;
            break;
        }
    }

    if (argc < 2) {
        usage();
        return 1;
    }

    const char* notes_path = resolve_notes_path(global);
    char* cmd = argv[1];

    // ── add ───────────────────────────────────────────────────────────────────
    if (!strcmp(cmd, "add")) {
        if (argc < 4) {
            printf("Usage: cpin add <file:line> \"<note>\" [--global]\n");
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
        err = fileio_save(&note, notes_path);
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
            err = fileio_load_all(notes_path, &result);
            if (err == CPIN_ERR_NOTE_NOT_FOUND || !result) {
                printf("No notes found\n");
                return 0;
            }
        } else {
            char* file = argv[2];
            char* line = (argc >= 4) ? argv[3] : NULL;
            err = fileio_load(file, line, notes_path, &result);
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
            printf("Usage: cpin remove <file:line> [--global]\n");
            return 1;
        }

        char* file = NULL;
        char* line = NULL;
        cpin_error_t err = parser_split_target(argv[2], &file, &line);
        if (err != CPIN_SUCCESS) {
            printf("Error: %s\n", error_to_string(err));
            return 1;
        }

        err = fileio_delete(file, line, notes_path);
        if (err != CPIN_SUCCESS) {
            printf("Error: %s\n", error_to_string(err));
            return 1;
        }

        printf("Note removed: %s:%s\n", file, line);
        return 0;
    }

    // ── search ────────────────────────────────────────────────────────────────
    if (!strcmp(cmd, "search")) {
        if (argc < 3) {
            printf("Usage: cpin search <keyword> [--global]\n");
            return 1;
        }

        char* result = NULL;
        cpin_error_t err = fileio_search(argv[2], notes_path, &result);
        if (err == CPIN_ERR_NOTE_NOT_FOUND || !result) {
            printf("No notes matching \"%s\"\n", argv[2]);
            return 0;
        }
        if (err != CPIN_SUCCESS) {
            printf("Error: %s\n", error_to_string(err));
            return 1;
        }

        printf("%s", result);
        free(result);
        return 0;
    }

    printf("Unknown command: %s\n", cmd);
    usage();
    return 1;
}
