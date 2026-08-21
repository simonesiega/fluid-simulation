# AGENTS.md

## Project

`fluid-simulation` is an interactive 2D fluid simulator and a first serious C++ / GPU-programming project.

Goals:
- learn modern C++ through a real project;
- build and understand a CPU reference solver first;
- implement OpenGL compute and NVIDIA CUDA backends;
- compare the GPU approaches with reproducible benchmarks;
- finish with strong documentation, demo media, tests, a Windows executable, and a v1.0.0 GitHub release.

## Stack

- C++20
- CMake
- raylib
- OpenGL 4.3+
- GLSL compute shaders
- NVIDIA CUDA

Prefer the existing stack. Add dependencies only when they clearly improve the project.

## Source of truth

When deciding what to do, use this order:

1. The user's current prompt.
2. The existing repository state.
3. [`.agents/ROADMAP.md`](.agents/ROADMAP.md) for project direction and phase boundaries.
4. The relevant skill under [`.agents/skills/`](.agents/skills/).

The roadmap describes the intended sequence, but it does not track live progress. Do not edit it merely to mark work complete.

If the user names a phase, milestone, or roadmap step, work within that scope. If the prompt is specific enough, do not ask for a current-phase update.

## Working rules

- Keep changes small and focused.
- Prefer clear code over premature abstraction or optimization.
- Keep the CPU solver as the reference implementation.
- Keep CPU, OpenGL Compute, and CUDA behavior comparable where practical.
- Avoid unnecessary CPU ↔ GPU transfers.
- Do not commit generated build output or release binaries.
- Keep Windows + MSVC as a first-class target.
- Update documentation when behavior, controls, build steps, architecture, or benchmarks change.
- Never invent benchmark numbers, screenshots, GIFs, release artifacts, or completed features.

## Task-specific guidance

Read only the skill relevant to the task:

- Build, CMake, dependencies, project layout: [`.agents/skills/build/SKILL.md`](.agents/skills/build/SKILL.md)
- Fluid math and CPU solver: [`.agents/skills/simulation/SKILL.md`](.agents/skills/simulation/SKILL.md)
- OpenGL, GLSL, CUDA, GPU resources: [`.agents/skills/gpu/SKILL.md`](.agents/skills/gpu/SKILL.md)
- Tests, parity checks, benchmarks: [`.agents/skills/testing/SKILL.md`](.agents/skills/testing/SKILL.md)
- README, docs, media, packaging, releases: [`.agents/skills/release/SKILL.md`](.agents/skills/release/SKILL.md)

## Validation

For code changes:
1. Configure successfully with CMake.
2. Build without errors.
3. Run the relevant backend/path.
4. Run affected tests.
5. Check for obvious visual or numerical regressions.
6. Run benchmarks only when performance behavior is intentionally being measured.
