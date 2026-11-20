# Project Rules

## Vendor is read-only — never touch `src/vendor/`

- No edits, no reformatting, no lint/format runs, no "small fixes" in vendored
  dependencies (spdlog, etc.). They stay byte-identical to the pinned version.
- All compatibility shims (fmt formatters, macro wrappers, patches applied via
  CMake flags) live in `src/engine/` — project code — instead.
- Never run a project-wide formatter without excluding `src/vendor/`.
- If vendor code seems to require a change, surface it to the user first;
  never make it silently.
