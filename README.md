# Tracker association: take-home

You own issue #1. Everything you need is in this repo, issue #1 and PR #2.
Nothing here is real project code or a real client.

## Build and test

```
./scripts/test.sh
```

Needs Docker. Builds in a container and runs the suite. Without Docker:
`cmake -S . -B build && cmake --build build && ./build/association_test`
with a C++17 compiler and CMake 3.16+.

## Read first

- `domain.md`: what a frame, detection, track and box are here.
- Issue #1: the bug and its acceptance lines.
- PR #2: an earlier attempt at this ticket, written by a coding agent. Its
  tests are green. Check it out
  with `git fetch origin pull/2/head:pr-2`.
- The comment on issue #1: the partner who reported it.

## Tasks

1. Review PR #2 against issue #1. Line comments in a file `review.md`, not on
   GitHub. Merge or not, and why. If not, end `review.md` with the
   instruction to the agent that would make it mergeable.
2. Fix #1 on `main`. Prove it with a test that fails before your fix and
   passes after. Commit the test and the fix separately.
3. PR description in `pr.md`, under 15 lines.
4. Reply to the partner thread on issue #1 in `reply.md`. One email.
5. `notes.md`, one paragraph: what you would do with another day, what you
   left alone on purpose, and one thing in this repo you had to look up or
   still do not understand.

## Rules

- Time box: 3–4 hours. Stop when you hit it and say so in `notes.md`.
- AI tools are allowed. Say in `notes.md` what they wrote.
- Clone, do not fork. Do not publish your work or comment on the issue or PR.
- Put `review.md`, `pr.md`, `reply.md` and `notes.md` at the repo root and
  commit them on `main` after the fix.
- Send back a tarball of your clone with your commits. Where and by when is
  in your invite email.
