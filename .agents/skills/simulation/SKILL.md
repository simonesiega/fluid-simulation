# Simulation skill

Use this skill for the CPU reference solver, grid/field representation, fluid mathematics, numerical behavior, simulation parameters, boundaries, and solver-side interaction semantics.

The CPU backend is the reference implementation and semantic contract for OpenGL Compute and CUDA.

## Priorities

1. Correctness and understanding.
2. Deterministic, stable behavior.
3. Clear data flow and explicit numerical choices.
4. Backend-comparable semantics.
5. Performance only after the reference behavior is trustworthy.

## Field and coordinate rules

- Keep simulation fields contiguous and dimensionally consistent.
- Centralize 2D-to-1D indexing rather than duplicating index arithmetic.
- Keep the viewport → normalized coordinates → grid coordinates mapping explicit.
- Define and preserve conventions for origin, cell centers, edge handling, and clamping.
- Use ping-pong/source-destination storage whenever a pass must read the previous field state.
- Never perform an in-place update merely to save memory if it changes the algorithm.

## Solver direction

Build and maintain the CPU solver around the roadmap's operations:

- force / velocity injection;
- RGB dye injection;
- semi-Lagrangian advection;
- diffusion;
- divergence;
- pressure solve;
- pressure-gradient subtraction / projection;
- dissipation and damping;
- vorticity and vorticity confinement;
- explicit domain boundaries;
- obstacle handling when that phase is reached.

Follow the current roadmap and existing CPU implementation for the exact step order. Do not silently reorder passes while porting or optimizing.

## Numerical rules

- Advance the solver with the project's fixed simulation timestep; rendering FPS must not redefine the simulation.
- Keep important coefficients and iteration counts named, configurable, and bounded to sensible ranges.
- Make boundary conditions explicit in code.
- Use the same interpolation and boundary semantics that GPU backends are expected to reproduce.
- Clear scratch/pressure state intentionally when the algorithm requires it.
- Guard against NaN/Inf propagation and invalid grid sizes/parameters.
- Prefer small functions/passes named after the numerical operation they perform.
- Avoid hidden constants that materially change the simulation.

## Reference-backend discipline

When adding or changing a numerical feature:

1. make the intended behavior clear in the CPU path first unless the roadmap explicitly says otherwise;
2. state the expected numerical and visible effect;
3. add/update deterministic tests when practical;
4. only then mirror the behavior in GPU backends.

Differences between backends must be intentional and documented, not accidental implementation drift.

## Claims

This project is a real-time visual fluid simulator, not validated CFD software.

Do not describe the simulation as physically exact or scientifically validated unless the repository later contains evidence supporting that claim.
