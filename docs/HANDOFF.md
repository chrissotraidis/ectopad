# Handoff

Last updated: 2026-08-11

## Where things stand

- Research phase in progress: environment, dump validation, repo pins, and
  architecture are documented (see STATUS/DEPENDENCIES/GAME_DATA/ARCHITECTURE).
- Next: Gate 1 (macOS ARM64 build) and `docs/IMPLEMENTATION_PLAN.md` are the
  immediate deliverables.

## How to resume

1. Read [STATUS.md](STATUS.md) first (status legend included).
2. Read [IMPLEMENTATION_PLAN.md](IMPLEMENTATION_PLAN.md) for the plan and gates.
3. Follow [BUILDING.md](BUILDING.md) to configure/build.
4. Append dated evidence to [TESTING.md](TESTING.md) and [PERFORMANCE.md](PERFORMANCE.md).

## Workspace invariants

- `ref/` and all game-derived data are git-ignored and must stay local.
- Before any commit, inspect staged files for game data, generated copyrighted
  files, saves, logs with sensitive data, provisioning profiles, certificates,
  or signing material.
- Preserve unrelated user changes; do not rewrite history unnecessarily.

