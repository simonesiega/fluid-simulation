# Testing and benchmarking skill

Use this skill for CTest/unit tests, numerical sanity checks, backend parity, regression tests, performance measurement, and benchmark documentation.

Testing establishes correctness evidence; benchmarking establishes performance evidence. Keep them separate.

## Deterministic tests

Prefer small deterministic cases over large visual-only checks.

CPU coverage should include relevant invariants such as:

- grid indexing and dimensions;
- clear/reset/resize/swap behavior;
- interpolation helpers;
- zero-field stability;
- advection on tiny known fields;
- divergence/projection sanity;
- boundary behavior;
- parameter validation;
- finite-value checks.

A good projection test should verify that projection reduces an agreed divergence metric on a controlled field rather than requiring arbitrary exact pressure values.

## GPU checks and parity

GPU checks should focus on:

- shader/kernel compilation and dispatch;
- tiny known inputs;
- reset/resource behavior;
- CPU ↔ OpenGL Compute parity;
- CPU ↔ CUDA parity;
- invariant/sanity checks when exact field parity is not meaningful.

- Compare floating-point values with justified tolerances, not exact equality.
- Keep parity scenarios deterministic and small enough to diagnose.
- Treat visual output as supporting evidence, not a substitute for a deterministic test when one is practical.
- Do not require unsupported GPU hardware for the portable CPU test suite.
- CUDA CI may compile the CUDA path without claiming runtime validation when no NVIDIA GPU is available.

## Regression discipline

When fixing a deterministic bug, add or strengthen a test that would have caught it when practical.

Do not make tests dependent on timing, frame rate, random input, or exact GPU floating-point bit patterns unless the test explicitly targets those properties.

## Benchmark methodology

Backend comparisons must use equivalent workloads.

For CPU/OpenGL Compute/CUDA comparisons:

- use Release builds;
- use the same grid size;
- use the same timestep;
- use the same pressure/diffusion iteration counts;
- use the same enabled solver features and preset/input scenario;
- use deterministic input rather than manual interaction;
- warm up before measurement;
- measure enough simulation steps to reduce noise;
- exclude startup, shader compilation, allocation, file loading, and other one-time work unless explicitly benchmarking them;
- synchronize only as required to measure the intended GPU work correctly.

Prefer simulation-step time in milliseconds as the primary metric. FPS may be shown only as a secondary user-facing metric.

Record enough context to reproduce the result:

- CPU and GPU model;
- backend;
- grid size;
- build configuration;
- compiler/toolchain;
- relevant driver/CUDA/OpenGL information when useful;
- solver parameters/iteration counts;
- warm-up method;
- measured step count;
- timing method;
- reported statistic.

Never compare different solver settings and label the result as a backend speedup.

## Benchmark artifacts

Store durable benchmark methodology/results under `docs/benchmarks/`, with machine-readable output (CSV/text) when the application supports exporting it.

Do not hand-invent, estimate, or manually "smooth" benchmark numbers. Do not commit large profiler captures unless they are intentionally useful evidence.
