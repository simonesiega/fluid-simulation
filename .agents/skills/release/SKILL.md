# Release skill

Use this skill for README work, documentation, screenshots, GIF/demo creation, packaging, CI, and GitHub releases.

## README

The finished README should quickly show:
- what the project is;
- a strong visual demo near the top;
- controls;
- how to download/build/run it;
- how the solver works at a high level;
- CPU vs OpenGL Compute vs CUDA design;
- measured benchmarks;
- limitations and future work.

Keep the learning story concise: this is a real project first, and a first C++ / GPU-programming experience second.

## Media

- Prefer a short readable GIF near the top of the README.
- Keep a higher-quality video/demo when useful.
- Capture real application output.
- Do not fake or mock final results.

## Release

Before `v1.0.0`:
- clean Release build;
- CPU/OpenGL/CUDA checks completed where supported;
- working Windows package;
- documented controls;
- screenshots/GIF verified;
- CI passing;
- benchmarks reproduced from the release commit;
- README links verified;
- license and third-party notices present;
- release notes written.

Do not commit large generated binaries to normal source history when GitHub Release assets are the appropriate destination.
