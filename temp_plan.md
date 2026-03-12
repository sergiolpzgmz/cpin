# Refactor Plan: Block-Format Storage

## Phase 1: Understand Before You Touch Anything

Before writing a single line, read all of `fileio.c` carefully and answer these questions for yourself:

- What does `parse_line()` do exactly? What are its failure cases?
- How does `fileio_load()` know which notes belong to a file?
- Why does `fileio_delete()` have to rewrite the entire file?
- What does `fileio_load_all()` return and in what format?
- What does `export --md` in `main.c` currently do to group notes, and why is it awkward?

Write down your answers. This ensures the new code solves real problems, not imagined ones.

---

## Phase 2: Design the New Parser First (on paper)

Before touching any C, sketch out the new parsing logic:

```
[src/main.c:NoteCount_inBlock]
10:first note
42:added later

[src/parser.c:NoteCount_InBlock]
7:some note
```

Ask yourself:
- How do you detect a block header vs. a note line?
- How do you know which file a note belongs to while reading line-by-line?
- What state does the parser need to track between lines?
- What happens with blank lines? Should you allow them?
- What happens if the file is empty?

Sketch a state machine on paper:
- State: `SEEKING_BLOCK` — skipping until you find `[file]`
- State: `IN_BLOCK` — reading `line:content` pairs

This mental model will make the C implementation mechanical.

---

## Phase 3: Write a Migration Function

**This is the most important step you could skip and regret.** Anyone using `cpin` today has the old format. You need a function:

```c
// Reads old format, writes new format, returns success/error
int fileio_migrate(const char* notes_path);
```

Strategy:
1. Use the existing `fileio_load_all()` to read all notes from the old format
2. Write them back out in the new grouped format
3. Call this from `fileio_initialize()` — detect the format and migrate automatically

Detection tip: if the first non-empty line starts with `[`, it's the new format. Otherwise it's old.

---

## Phase 4: Rewrite `fileio.c` Function by Function

Tackle functions in this order — each one is simpler to test than the last:

### 4a. `parse_line()` — internal helper
This changes the most. Instead of parsing `file:line:content`, it now parses `line:content` (the file is tracked externally by the block header parser). Rewrite this first since everything depends on it.

### 4b. `fileio_save()`
New behavior:
1. Check if `[filename]` block already exists in the file
2. If yes — append the new `line:content` inside that block
3. If no — append a new `[filename]` block at the end

Challenge: inserting into the middle of a file requires reading it all, modifying in memory, and rewriting. Same cost as `fileio_delete()` already pays today.

### 4c. `fileio_load()` and `fileio_load_all()`
New behavior: use the state machine from Phase 2. `fileio_load(file)` can stop reading as soon as it exits the matching block — that's the performance win.

### 4d. `fileio_search()`
Now gets to skip entire blocks early if optimizing. Start simple: just adapt the new parser, optimization is optional.

### 4e. `fileio_delete()`
Read entire file, skip the matching `line:content` inside the matching `[file]` block, rewrite. If a block becomes empty after deletion, remove the block header too.

---

## Phase 5: Update `main.c` Export

The `export --md` command currently groups notes itself with awkward deduplication logic. With the new format, notes are already grouped by file in storage — the markdown export becomes:

```
for each block in notes file:
    print ## filename
    for each note in block:
        print - Line X: content
```

This is a simplification, not a rewrite. `main.c` should get shorter, not longer.

---

## Phase 6: Test Everything Manually

Create a test `.cpin/notes` file in the old format, run `cpin list`, verify auto-migration happens. Then test:

| Command | Expected outcome |
|---|---|
| `cpin add src/main.c:10 "note"` | Creates new block |
| `cpin add src/main.c:42 "another"` | Appends to existing block |
| `cpin add src/parser.c:7 "note"` | Creates second block |
| `cpin list src/main.c` | Shows only main.c notes |
| `cpin remove src/main.c:10` | Removes one note, keeps block |
| `cpin remove src/main.c:42` | Removes block entirely |
| `cpin export --md` | Clean grouped output |

---

## Key Pitfalls to Watch For

1. **Off-by-one in block detection** — `[file]` headers must be matched exactly, not as substrings
2. **Empty blocks** — decide upfront: remove them, or leave them? (Removing is cleaner)
3. **Notes with colons in content** — `parse_line()` must split only on the *first* colon, same as today
4. **Global vs local path** — nothing changes here, but make sure migration runs for both
5. **File handles** — don't forget `fclose()` on all paths, including error paths

---

## Suggested Commit Sequence

```
feat: add new block-format parser to fileio.c
feat: add migration function for old flat format
feat: rewrite fileio_save to use block format
feat: rewrite fileio_load/load_all for block format
feat: rewrite fileio_delete for block format
feat: simplify export --md using pre-grouped storage
```

Small commits make it easier to bisect if something breaks.
