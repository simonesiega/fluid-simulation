# Simulation skill

Use this skill for the CPU solver, fluid mathematics, numerical methods, fields, and simulation behavior.

## Priorities

1. Correctness and understanding.
2. Stable visual behavior.
3. Clear data flow.
4. Performance only after the reference implementation works.

## Solver direction

Build the fluid solver incrementally around:
- velocity and density/RGB dye fields;
- force injection;
- semi-Lagrangian advection;
- diffusion where used;
- divergence;
- pressure solve;
- pressure-gradient subtraction / projection;
- dissipation;
- vorticity confinement;
- explicit boundary handling.

Keep the CPU path understandable enough to serve as the reference for both GPU backends.

## Rules

- Make boundary conditions explicit.
- Keep important simulation parameters named and tunable.
- Avoid hidden constants that materially affect solver behavior.
- Prefer small testable functions/passes.
- When changing numerical behavior, state the expected visual/numerical effect.
- Do not call the result physically accurate unless it has been validated as such.
