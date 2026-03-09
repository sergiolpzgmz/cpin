#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include "errors.h"
#include "fileio.h"

#define CPIN_DIR   ".cpin"
#define CPIN_NOTES ".cpin/notes"

// ── note lifecycle ────────────────────────────────────────────────────────────

cpin_note_t fileio_create_note(char* file, char* line, char* content) {
    cpin_note_t note;
    note.file = file    ? strdup(file)    : NULL;
    note.line = line    ? strdup(line)    : NULL;
    note.content = content ? strdup(content) : NULL;
    return note;
}

void fileio_note_free(cpin_note_t* note) {
    if (!note) return;
    free(note->file);
    free(note->line);
    free(note->content);
    note->file = note->line = note->content = NULL;
}

// ── storage helpers ───────────────────────────────────────────────────────────

// Ensures the .cpin directory exists.
cpin_error_t fileio_initialize(cpin_note_t* node) {
    (void)node;
    struct stat st = {0};
    if (stat(CPIN_DIR, &st) == -1) {
        if (mkdir(CPIN_DIR, 0755) != 0)
            return CPIN_ERR_STORAGE_INIT;
    }
    return CPIN_SUCCESS;
}

// ── single-file storage ───────────────────────────────────────────────────────
//
// Every note occupies exactly one line in .cpin/notes:
//
//   src/memory.c:87:potential leak here
//
// The file and line fields may not contain ':'.
// The content field may contain ':' — we only split on the first two.

// Appends a note to .cpin/notes.
cpin_error_t fileio_save(cpin_note_t* node) {
    if (!node || !node->file || !node->content) return CPIN_ERR_INVALID_ARGS;

    cpin_error_t err = fileio_initialize(node);
    if (err != CPIN_SUCCESS) return err;

    FILE* f = fopen(CPIN_NOTES, "a");
    if (!f) return CPIN_ERR_WRITE_FAILED;

    if (node->line && node->line[0] != '\0')
        fprintf(f, "%s:%s:%s\n", node->file, node->line, node->content);
    else
        fprintf(f, "%s::%s\n", node->file, node->content);

    fclose(f);
    return CPIN_SUCCESS;
}

// Parses one raw storage line (NULL-terminated, may include '\n') into parts.
// Splits only on the first two ':' so content may contain ':'.
// Returns 0 on success, -1 if the line is malformed.
static int parse_line(char* raw, char** out_file, char** out_line, char** out_content) {
    char* first = strchr(raw, ':');
    if (!first) return -1;
    *first = '\0';
    *out_file = raw;

    char* second = strchr(first + 1, ':');
    if (!second) return -1;
    *second = '\0';
    *out_line = first + 1;

    // Strip trailing newline from content
    char* content = second + 1;
    size_t len = strlen(content);
    if (len > 0 && content[len - 1] == '\n') content[len - 1] = '\0';
    *out_content = content;

    return 0;
}

// Loads notes matching file (and optionally line) into a newly allocated
// string (*result).  Caller must free(*result).
cpin_error_t fileio_load(char* file, char* line, char** result) {
    if (!file || !result) return CPIN_ERR_INVALID_ARGS;
    *result = NULL;

    FILE* f = fopen(CPIN_NOTES, "r");
    if (!f) return CPIN_ERR_NOTE_NOT_FOUND;

    char buf[4096];
    char* out = NULL;
    size_t out_len = 0;

    while (fgets(buf, sizeof(buf), f)) {
        char tmp[4096];
        strncpy(tmp, buf, sizeof(tmp) - 1);
        tmp[sizeof(tmp) - 1] = '\0';

        char *tok_file, *tok_line, *tok_content;
        if (parse_line(tmp, &tok_file, &tok_line, &tok_content) != 0) continue;
        if (strcmp(tok_file, file) != 0) continue;
        if (line && strcmp(tok_line, line) != 0) continue;

        // Re-assemble display line: file:line:content\n
        char display[4096];
        int n = snprintf(display, sizeof(display), "%s:%s:%s\n",
                         tok_file, tok_line, tok_content);
        if (n <= 0) continue;

        char* tmp_out = realloc(out, out_len + (size_t)n + 1);
        if (!tmp_out) { free(out); fclose(f); return CPIN_ERR_WRITE_FAILED; }
        out = tmp_out;
        memcpy(out + out_len, display, (size_t)n);
        out_len += (size_t)n;
        out[out_len] = '\0';
    }

    fclose(f);
    *result = out;
    return (out_len > 0) ? CPIN_SUCCESS : CPIN_ERR_NOTE_NOT_FOUND;
}

// Loads all notes from .cpin/notes into a newly allocated string (*result).
// Caller must free(*result).
cpin_error_t fileio_load_all(char** result) {
    if (!result) return CPIN_ERR_INVALID_ARGS;
    *result = NULL;

    FILE* f = fopen(CPIN_NOTES, "r");
    if (!f) return CPIN_ERR_NOTE_NOT_FOUND;

    char buf[4096];
    char* out = NULL;
    size_t out_len = 0;

    while (fgets(buf, sizeof(buf), f)) {
        char tmp[4096];
        strncpy(tmp, buf, sizeof(tmp) - 1);
        tmp[sizeof(tmp) - 1] = '\0';

        char *tok_file, *tok_line, *tok_content;
        if (parse_line(tmp, &tok_file, &tok_line, &tok_content) != 0) continue;

        char display[4096];
        int n = snprintf(display, sizeof(display), "%s:%s:%s\n",
                         tok_file, tok_line, tok_content);
        if (n <= 0) continue;

        char* tmp_out = realloc(out, out_len + (size_t)n + 1);
        if (!tmp_out) { free(out); fclose(f); return CPIN_ERR_WRITE_FAILED; }
        out = tmp_out;
        memcpy(out + out_len, display, (size_t)n);
        out_len += (size_t)n;
        out[out_len] = '\0';
    }

    fclose(f);
    *result = out;
    return (out_len > 0) ? CPIN_SUCCESS : CPIN_ERR_NOTE_NOT_FOUND;
}

// Deletes all notes matching file:line from .cpin/notes (rewrites the file).
cpin_error_t fileio_delete(char* file, char* line) {
    if (!file) return CPIN_ERR_INVALID_ARGS;

    FILE* f = fopen(CPIN_NOTES, "r");
    if (!f) return CPIN_ERR_NOTE_NOT_FOUND;

    // Read all raw lines into memory
    char** lines = NULL;
    size_t count = 0;
    char buf[4096];

    while (fgets(buf, sizeof(buf), f)) {
        char** grown = realloc(lines, (count + 1) * sizeof(char*));
        if (!grown) {
            for (size_t i = 0; i < count; i++) free(lines[i]);
            free(lines);
            fclose(f);
            return CPIN_ERR_WRITE_FAILED;
        }
        lines = grown;
        lines[count++] = strdup(buf);
    }
    fclose(f);

    // Rewrite without matching lines
    FILE* out = fopen(CPIN_NOTES, "w");
    if (!out) {
        for (size_t i = 0; i < count; i++) free(lines[i]);
        free(lines);
        return CPIN_ERR_WRITE_FAILED;
    }

    int deleted = 0;
    for (size_t i = 0; i < count; i++) {
        char tmp[4096];
        strncpy(tmp, lines[i], sizeof(tmp) - 1);
        tmp[sizeof(tmp) - 1] = '\0';

        char *tok_file, *tok_line, *tok_content;
        int malformed = parse_line(tmp, &tok_file, &tok_line, &tok_content);

        int match = (!malformed && strcmp(tok_file, file) == 0);
        if (match && line) match = (strcmp(tok_line, line) == 0);

        if (!match)
            fputs(lines[i], out);
        else
            deleted = 1;

        free(lines[i]);
    }
    free(lines);
    fclose(out);

    return deleted ? CPIN_SUCCESS : CPIN_ERR_NOTE_NOT_FOUND;
}
