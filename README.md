# Linux Directory Watch

Stream immediate directory changes as JSON Lines, including paired rename cookies and explicit overflow notices.

[![Build and verify](https://github.com/netcodermike/linux-directory-watch/actions/workflows/verify.yml/badge.svg)](https://github.com/netcodermike/linux-directory-watch/actions/workflows/verify.yml)

**Language:** C · **Platform:** Linux · **App type:** Linux filesystem event recorder

First-release learning implementation, with explicit scope below. This is one independently runnable app, not a placeholder in a combined repository.

## Features

- Create, close-after-write, delete, move, and attribute events.
- JSON Lines with timestamp, name, rename cookie, and directory flag.
- Nonblocking inotify plus poll, with SIGINT/SIGTERM shutdown.
- Queue-overflow diagnostics and watch-removal handling.

## Requirements

Linux inotify, a C11 compiler, make, and Python 3 for the integration test. No root privileges.

## Quick start

```sh
git clone https://github.com/netcodermike/linux-directory-watch.git
cd linux-directory-watch
make
./dirwatch /path/to/watch
# Save events outside the watched folder to avoid a feedback loop:
./dirwatch /path/to/watch > /another/path/events.jsonl
```

Commands run from the repository root. Lines marked for a particular operating system apply only there. Language tools are prerequisites, not installed by the app. Close desktop windows normally; terminal/server apps stop with Ctrl+C unless their usage notes say otherwise.

## Example workflow

Watch an empty test directory. In another terminal create a file, write to it, rename it, and delete it. moved-from and moved-to should share a nonzero cookie. Create a subdirectory; its creation appears, but nested files inside it are outside this single-level watch.

## Architecture, configuration, and data

`dirwatch.c` opens an inotify descriptor, installs a single directory watch, then polls/reads aligned variable-length event records. Output is line-buffered JSON. The test creates, writes, renames, and deletes a fixture, verifies matching rename cookies and quoted-name escaping, then terminates the process. Filenames are represented bytewise: ASCII is preserved, other bytes are escaped as U+00xx so arbitrary Linux filename bytes still yield valid JSON. Reconstruct non-ASCII original bytes by mapping those code points back to bytes; this is not UTF-8 text normalization.

## Verification

Build with the quick-start command where needed, then run:

```sh
make test
```

Manual acceptance: Run make test on Linux, then manually inspect attribute changes, directory creation, a moved/deleted watched root, and signal shutdown. Watch a small synthetic directory before using a busy one.

See [validation record](docs/VALIDATION.md) for observed checks and their limits. CI results verify only what the workflow runs; they do not establish native GUI correctness or production readiness.

## Limitations and next steps

Not recursive, not a file-content/version backup, and not a guaranteed audit trail. Events can coalesce, race, or overflow; an overflow requires a separate directory rescan. No initial snapshot or automatic watch reattachment. Timestamps are receipt times in whole epoch seconds. Output consumers must handle the bytewise filename encoding. Writing the event log inside the watched directory can create a feedback loop.

## Troubleshooting

inotify_add_watch errors commonly indicate a missing directory, permissions, or exhausted watch limits. Use a directory rather than a file. If a watched directory is deleted the watch is removed. Queue overflow is printed to stderr and emitted in JSON. Ctrl+C or SIGTERM stops the watcher.

## Source map

- `dirwatch.c`
- `Makefile`
- `tests/integration.py`

## Development and license

Read [contributing](CONTRIBUTING.md) before changing the app, and [security/privacy](SECURITY.md) before supplying private data. Use synthetic fixtures for tests. No telemetry, paid API key, or account is required by the app itself. Dependencies, when present, are stated in Requirements.

[MIT license](LICENSE), copyright 2026 netcodermike.
