# Release skill

Use this skill for README/documentation work, screenshots and demo media, CI, install/package layout, release assets, changelog/version work, and the `v0.0.1` GitHub release.

Do not use final documentation to claim features that do not yet exist.

## Documentation timing

Behavior-specific documentation may be updated whenever behavior changes, but the **final** README, architecture story, three-backend benchmark presentation, screenshots, and demo media belong after CPU, OpenGL Compute, and CUDA are complete and validated.

This avoids producing "final" documentation that immediately becomes stale when CUDA is added.

## README

The finished README should let a new visitor understand the project quickly. Prefer this order:

1. title and one-line description;
2. strong real demo near the top;
3. what the project is and why it exists;
4. features;
5. download / quick start / build from source;
6. controls;
7. CPU vs OpenGL Compute vs CUDA architecture summary;
8. measured benchmark table;
9. short solver explanation;
10. deeper documentation links / project structure;
11. concise learning story;
12. limitations / future work;
13. license / acknowledgements.

Keep the project story engineering-first: the first C++/GPU-programming experience is useful context, not the main product claim.

## Technical documentation

Final documentation should cover, as applicable:

- architecture and data flow;
- fluid math;
- OpenGL Compute implementation;
- CUDA implementation;
- backend comparison and trade-offs;
- build/troubleshooting;
- controls/presets;
- benchmark methodology/results.

Documentation must agree with the actual source, controls, build options, and measured data.

## Media

- Capture real application output only.
- Prefer a short optimized GIF/WebP near the top of the README.
- Keep a higher-quality MP4 as a release/portfolio asset when useful.
- Use clean states that demonstrate the finished interaction and backend story.
- Do not mock, fake, or fabricate screenshots, benchmark charts, or demo output.
- Keep large raw recordings out of normal Git history.

## CI

The release path should eventually include:

- Windows/MSVC portable configure/build;
- CPU CTest execution;
- a separate CUDA-enabled compile job when practical;
- no claim of CUDA runtime validation on hosted runners without suitable NVIDIA hardware;
- tagged release automation only after packaging is deterministic.

## Packaging

- Base the Windows package on CMake install/output rules rather than manual copying.
- Include the executable, required shaders/resources/assets, license, and required third-party notices.
- Keep CUDA optional at runtime/build time according to the project's supported package design.
- Verify the ZIP from a fresh extracted directory, not from the repository working tree.

## v0.0.1 gate

Before publishing `v0.0.1`, verify from the intended release commit:

- clean Release build;
- CPU tests pass;
- OpenGL Compute works on supported hardware;
- CUDA works on supported NVIDIA hardware;
- backend switching and core controls work;
- packaged runtime resources resolve correctly;
- CPU/OpenGL/CUDA benchmarks were regenerated with documented methodology;
- README screenshots/demo/links render correctly;
- CI is green;
- changelog/version are correct;
- license and third-party notices are present;
- the Windows x64 ZIP launches independently.

Do not publish or describe a release as verified when any required check was only assumed.
