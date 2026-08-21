# GPU skill

Use this skill for OpenGL, GLSL compute shaders, CUDA, GPU resources, synchronization, and GPU backend work.

## Backend goals

- OpenGL Compute is the portable GPU backend.
- CUDA is an optional NVIDIA-specific backend.
- The CPU solver remains the reference implementation.
- Keep simulation data resident on the GPU whenever practical.
- Match solver semantics across backends before optimizing.

## Preferred resource layout

Use GPU resources according to access pattern rather than convenience.

### Textures / images

Prefer textures or image load/store when:
- 2D spatial sampling is important;
- bilinear filtering is useful for advection;
- the data naturally represents a 2D field;
- the same resource is also convenient for visualization.

Likely candidates:
- velocity field;
- RGB dye field;
- visualization/display texture.

### SSBOs / linear buffers

Prefer SSBOs or CUDA linear device buffers when:
- access is explicit and index-based;
- filtering is not needed;
- the data is scalar/scratch/intermediate state;
- linear memory makes parity or CUDA sharing clearer.

Likely candidates:
- pressure;
- divergence;
- curl/vorticity;
- temporary/debug buffers.

This is a default convention, not a hard rule. Change it only when profiling or implementation clarity gives a concrete reason.

## Ping-pong resources

Never read from and write to the same field in a pass when the algorithm requires the previous state.

Use clearly paired resources, for example:

```text
velocity_a / velocity_b
dye_a      / dye_b
pressure_a / pressure_b
```

Swap logical source/destination handles after each pass or iteration rather than copying buffers.

## Shader naming

Keep one main responsibility per compute shader.

Preferred naming:

```text
inject_dye.comp
inject_velocity.comp
advect_velocity.comp
advect_dye.comp
diffuse_velocity.comp
diffuse_dye.comp
divergence.comp
pressure.comp
gradient.comp
vorticity.comp
vorticity_confinement.comp
boundary.comp
```

Name CUDA kernels/functions using the same conceptual vocabulary where practical so backend comparison stays easy.

## Synchronization

- Use explicit OpenGL memory barriers where subsequent passes depend on previous writes.
- Check shader compilation/linking errors and surface useful diagnostics.
- Check CUDA API and kernel-launch errors.
- Avoid `glFinish`, device-wide synchronization, or CPU readback unless required for correctness, diagnostics, or measurement.

## Performance

Measure before optimizing.

When reporting results, include:
- hardware;
- backend;
- grid size;
- Release build configuration;
- timestep and relevant solver settings;
- pressure/diffusion iteration counts;
- warm-up method;
- number of measured frames/steps;
- metric being reported.

Never invent performance data.
