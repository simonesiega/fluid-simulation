# GPU skill

Use this skill for OpenGL 4.3 compute shaders, GLSL, GPU resource ownership, dispatch, synchronization, CUDA kernels, CUDA/OpenGL interoperability, and GPU backend behavior.

Do not use GPU implementation choices to redefine the solver. The CPU backend remains the reference for semantics.

## Backend contract

- OpenGL Compute is the portable GPU backend.
- CUDA is an optional NVIDIA-specific backend.
- Match CPU behavior before optimizing.
- Keep full simulation fields GPU-resident whenever practical.
- Avoid full per-frame CPU readback; only move small diagnostics/results when needed.
- A selected GPU backend must actually execute that backend rather than silently falling back to CPU simulation.

## Shared semantics

CPU, OpenGL Compute, and CUDA should agree where practical on:

- grid dimensions and coordinate conventions;
- brush footprint and injection scaling;
- timestep and solver parameters;
- interpolation/backtracing rules;
- pressure/diffusion iteration semantics;
- dissipation/damping;
- boundaries and obstacles;
- logical solver pass order.

Floating-point results may differ; behavior and invariants should remain comparable.

## OpenGL Compute

- Require/detect OpenGL 4.3 compute capability before enabling the backend.
- Surface useful shader compile/link/load errors with the relevant shader name.
- Keep one main numerical responsibility per compute shader.
- Use work-group sizes deliberately; dispatch enough groups to cover the grid and guard out-of-range invocations in the shader.
- Keep image/buffer bindings and uniforms explicit and easy to trace.
- Use ping-pong resources whenever a pass requires the previous state.
- Insert the narrowest correct memory barrier before dependent compute/render operations.
- Do not use `glFinish` as normal synchronization.

### Resource choice

Choose representation from the actual access pattern and existing architecture:

- textures/images are a strong default for 2D fields that need spatial sampling, interpolation, or direct visualization;
- SSBOs/linear buffers can be appropriate for explicit index-based scalar/scratch data.

Do not migrate a field between representations without a concrete correctness, clarity, interoperability, or measured-performance reason.

## CUDA

- Keep CUDA code isolated behind the optional CUDA build/backend path.
- Use explicit ownership and cleanup for device resources.
- Check CUDA runtime calls and kernel launches with actionable diagnostics.
- Use the same conceptual pass/kernel vocabulary as CPU/OpenGL where practical.
- Preserve source/destination separation for iterative/advection operations.
- Do not introduce device-wide synchronization in the steady-state loop unless correctness or measurement requires it.
- For CUDA/OpenGL interop, make registration, mapping, access, unmapping, and destruction lifetimes explicit and balanced.
- Unsupported systems must hide/disable CUDA cleanly without breaking CPU/OpenGL use.

## Performance

Correctness and parity come first.

For performance claims, use the testing/benchmarking skill. Prefer GPU-appropriate timing (for example OpenGL timer queries or CUDA events when implemented) rather than treating CPU dispatch-call duration as GPU execution time.

Never invent a speedup or infer one from visual smoothness alone.
