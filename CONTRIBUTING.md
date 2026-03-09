# Contributing to cpin

Thanks for your interest in contributing! cpin is a small C project and contributions of all sizes are welcome — bug fixes, new features, docs, or packaging.

## Getting Started

### Prerequisites

- `clang` or `gcc`
- `make`
- A POSIX-compatible system (Linux or macOS)

### Build from source

```bash
git clone https://github.com/jonaebel/cpin.git
cd cpin
make
./cpin
```

### Project structure

```
cpin/
├── src/
│   ├── main.c       # Entry point, command dispatch
│   ├── parser.c     # Argument parsing (file:line splitting)
│   ├── fileio.c     # Note storage (read/write .cpin/notes)
│   └── errors.c     # Error codes and messages
├── include/
│   ├── cpin.h       # Shared types (cpin_note_t)
│   ├── parser.h
│   ├── fileio.h
│   └── errors.h
├── Makefile
└── README.md
```

Notes are stored in `.cpin/notes` in the project root using the format `file:line:content`.

## How to Contribute

### 1. Pick an issue

Check the [open issues](https://github.com/YOUR_USERNAME/cpin/issues). Issues tagged `good first issue` are a great starting point if you're new to the codebase.

### 2. Fork and branch

```bash
git checkout -b feat/your-feature-name
# or
git checkout -b fix/your-bug-name
```

### 3. Make your changes

- Keep changes focused — one feature or fix per PR
- Follow the existing code style (see below)
- Test your changes manually before submitting

### 4. Open a Pull Request

Push your branch and open a PR against `main`. Fill out the PR template and describe what you changed and why.

---

## Code Style

- C99, compiled with `-Wall -Wextra`
- 4-space indentation
- Snake case for all functions and variables: `fileio_save`, `cpin_note_t`
- Prefix functions with their module name: `parser_*`, `fileio_*`
- Keep functions small and single-purpose
- Free what you allocate — no memory leaks
- Error handling via `cpin_error_t` return codes (see `include/errors.h`)

---

## Reporting Bugs

Use the [bug report template](.github/ISSUE_TEMPLATE/bug_report.md) when opening an issue.

## Requesting Features

Use the [feature request template](.github/ISSUE_TEMPLATE/feature_request.md). Check existing issues first to avoid duplicates.

---

## License

By contributing you agree that your contributions will be licensed under the [MIT License](LICENSE).
