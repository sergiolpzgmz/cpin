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
- [ ] Basic add/list/remove
- [ ] Search across project
- [ ] TUI interface
- [ ] Neovim plugin
