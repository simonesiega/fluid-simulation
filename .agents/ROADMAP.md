# Project roadmap

This file gives agents permanent high-level project direction.

It is **not** a progress tracker. Do not add current-phase markers, completion checkboxes, dates, or status fields here. The user's prompt and repository state determine the current work.

For the detailed commit-by-commit plan, use the project roadmap PDF maintained outside this lightweight agent context.

## Phase 1 — Application shell and input loop

Create the persistent window, frame loop, input handling, viewport behavior, and basic application structure.

End state: the application responds cleanly to keyboard and mouse input.

## Phase 2 — CPU grid, fields, and interaction

Introduce the simulation grid and CPU-side velocity/dye fields, indexing, resets, and mouse-driven field injection.

End state: values can be injected into and visualized from a CPU grid.

## Phase 3 — CPU reference fluid solver

Build the understandable reference implementation:
- advection;
- diffusion;
- divergence;
- pressure solve;
- projection;
- dissipation;
- vorticity confinement;
- boundary handling.

End state: a stable interactive CPU fluid simulation exists.

## Phase 4 — CPU rendering, controls, and baseline performance

Render the CPU fields efficiently, add runtime controls/presets/HUD, and establish deterministic CPU timing and benchmark behavior.

End state: the reference backend is usable, measurable, and documented.

## Phase 5 — OpenGL compute foundation

Require/detect OpenGL compute support, load GLSL compute shaders, allocate GPU fields, establish ping-pong resources, barriers, and direct GPU rendering.

End state: a compute shader modifies GPU-resident simulation data and the result renders without full CPU round-trips.

## Phase 6 — OpenGL compute fluid solver

Port the CPU solver into clear GPU compute passes while preserving equivalent simulation semantics.

End state: the full interactive solver runs on OpenGL compute shaders.

## Phase 7 — Final interaction and visual features

Unify backend switching and finish the user-facing simulation:
- RGB dye interaction;
- presets;
- parameter controls;
- obstacles;
- pause/reset/help;
- visual polish.

End state: the application feels like a finished interactive demo rather than a solver prototype.

## Phase 8 — Correctness, robustness, and performance evidence

Add deterministic tests, backend parity/sanity checks, failure handling, stable timing, and reproducible benchmark methodology.

End state: correctness claims and performance comparisons have evidence.

## Phase 9 — Documentation and demo media

Create the technical documentation, screenshots, recording, optimized README GIF, architecture explanation, controls, build guide, and learning story.

End state: a new visitor can understand the project without reading the source first.

## Phase 10 — NVIDIA CUDA backend

Add CUDA as an optional NVIDIA-specific implementation of the same solver:
- optional CUDA CMake path;
- device/resource management;
- equivalent solver kernels;
- CUDA/OpenGL interoperability;
- backend selection;
- parity checks;
- OpenGL Compute vs CUDA benchmarking.

CUDA must remain optional. Non-CUDA builds must continue to work.

End state: NVIDIA hardware can run both GPU approaches under equivalent settings.

## Phase 11 — CI, packaging, and v1.0.0 release

Add automated portable builds/tests, CUDA compile validation, runtime-resource packaging, Windows x64 release layout, GitHub release automation, notices, release docs, and final manual validation.

End state: `v1.0.0` is public and downloadable with verified release assets.

Do not expand v1.0 scope with these unless the user explicitly changes the plan.
