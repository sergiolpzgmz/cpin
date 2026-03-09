# cpin 
A sticky note tool for your code which Attaches notes to specific 
lines and saves your notes localy without touching your source files.

## Usage
```bash
# Add a note to line 42 in parser.c
cpin add src/parser.c:42 "why does this loop start at 1?"

# List all notes for a specific file
cpin list src/parser.c

# Remove a note from line 42 in parser.c
cpin remove src/parser.c:42

```

## Installation
Currently under development. Installation instructions will be available once the tool is ready.

## Roadmap

### v0.1 — Core (in progress)
- [x] `add`, `list`, `remove` commands
- [x] Single-file storage (`.cpin/notes`) in `file:line:content` format
- [ ] `make install` target
- [x] `cpin list` with no arguments (show all notes in project)

### v0.2 — Search & Export
- [ ] `cpin search <keyword>` — grep across all notes
- [ ] `cpin export` — print notes as Markdown or JSON
- [ ] Per-project (`.cpin/`) vs global (`~/.cpin/`) storage via flag

### v0.3 — Editor Integration
- [ ] Git hook: warn when a noted line has moved or been deleted
- [ ] TUI interface (ncurses) for browsing notes
- [ ] Neovim plugin — show inline virtual text for noted lines
- [ ] VSCode extension

### v1.0 — Open Source Release
- [ ] `brew` formula / package manager distribution
- [ ] Stable CLI interface with man page
- [ ] CI/CD pipeline with tests
