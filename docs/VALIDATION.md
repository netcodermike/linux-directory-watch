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

## Observed CI result

Passed on November 6, 2026: [target-platform build/check](https://github.com/netcodermike/linux-directory-watch/actions/runs/42198937114).

Verified source commit: `35a3b5f692600ba02d0d4046f88e35e6bc3e33f2`. This evidence identifies the exact tested source; a later documentation-only commit does not change that source.

The commands listed above passed. Native GUI/manual acceptance is not claimed by this CI result.
