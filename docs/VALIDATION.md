# Validation

Authoring date: November 6, 2026. Host: Windows. Target: Linux.

## Local result

Linux inotify is unavailable on the authoring host. CI compiles with strict warnings and runs real filesystem/signal integration checks.

## Reproducible checks

The GitHub Actions workflow builds/runs the following on `ubuntu-latest`:

```sh
make
make test
```

## Manual acceptance

Run make test on Linux, then manually inspect attribute changes, directory creation, a moved/deleted watched root, and signal shutdown. Watch a small synthetic directory before using a busy one.

A workflow file is not evidence of a passing run. Observed CI run links and source commits are recorded after publication. Native GUI, sound, sleep/resume, and platform permissions require direct target-platform acceptance even when model tests pass.
