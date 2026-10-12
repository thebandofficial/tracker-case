# Tracker association: take-home

This repo is a small C++ object tracker and a hiring take-home. A detector
finds boxes in each camera frame. The tracker links those boxes from frame
to frame and gives each object an ID that stays the same over time. Nothing
here is real project code or a real client.

There is a bug: two objects that cross can swap IDs. It is
[issue #1](https://github.com/thebandofficial/tracker-case/issues/1), and you
own it. Everything you need is in this repo, issues #1 and
[#3](https://github.com/thebandofficial/tracker-case/issues/3), and
[PR #2](https://github.com/thebandofficial/tracker-case/pull/2).

## Build and test

```
./scripts/test.sh
```

[`scripts/test.sh`](scripts/test.sh) needs Docker. It builds in a container
and runs the tests. Without Docker you need a C++17 compiler and CMake 3.16+:
`cmake -S . -B build && cmake --build build && ./build/association_test`

## Read first

- [`domain.md`](domain.md): the words used here (frame, detection, box,
  track) and the matching rules.
- [Issue #1](https://github.com/thebandofficial/tracker-case/issues/1): the
  bug. Its "Acceptance" list says what a fix has to do.
- [PR #2](https://github.com/thebandofficial/tracker-case/pull/2): an earlier
  fix for issue #1, written by a coding agent. Its tests pass.
  Get it locally with `git fetch origin pull/2/head:pr-2`.
- [The comment on issue #1](https://github.com/thebandofficial/tracker-case/issues/1#issuecomment-6109420639):
  emails from the partner, the customer who reported the bug.
- [Issue #3](https://github.com/thebandofficial/tracker-case/issues/3): the
  system design task.
- Code: [`include/tracker/association.hpp`](include/tracker/association.hpp),
  [`src/association.cpp`](src/association.cpp),
  [`tests/association_test.cpp`](tests/association_test.cpp).

## Tasks

1. Review [PR #2](https://github.com/thebandofficial/tracker-case/pull/2)
   against [issue #1](https://github.com/thebandofficial/tracker-case/issues/1).
   Write your line comments in `review.md`, not on GitHub. Say whether you
   would merge it, and why. If not, end `review.md` with the instruction you
   would give the agent so its next attempt is mergeable.
2. Fix [issue #1](https://github.com/thebandofficial/tracker-case/issues/1)
   on `main`. Add a test that fails before your fix and passes after. Commit
   the test and the fix separately.
3. Write the PR description for your fix in `pr.md`, under 15 lines.
4. Answer the
   [partner's emails on issue #1](https://github.com/thebandofficial/tracker-case/issues/1#issuecomment-6109420639)
   in `reply.md`. One email.
5. Write `notes.md`, one paragraph: what you would do with another day, what
   you left alone on purpose, and one thing in this repo you had to look up
   or still do not understand.
6. Read [`clocks-excerpt.md`](clocks-excerpt.md), a page from a library you
   may not know. Nothing to hand in. We ask about it in the interview.
7. Draw the system described in
   [issue #3](https://github.com/thebandofficial/tracker-case/issues/3) and
   pick its architecture. We discuss your drawing in the interview.

## Rules

- Time box: 3–4 hours. Stop when you hit it and say so in `notes.md`.
- AI tools are allowed. Say in `notes.md` which parts they wrote.
- Clone, do not fork. Do not publish your work, and do not comment on the
  [issues](https://github.com/thebandofficial/tracker-case/issues) or the
  [PR](https://github.com/thebandofficial/tracker-case/pull/2).
- Put `review.md`, `pr.md`, `reply.md`, `notes.md` and `arch.md` or
  `arch.png` at the repo root. Commit them on `main` after the fix.
- Send back a tarball of your clone with your commits. Where and by when is
  in your invite email.
