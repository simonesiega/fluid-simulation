#pragma once

namespace fluid_simulation::simulation {
/**
 * @brief Configures the simulation grid, numerical solver, and interaction brush.
 */
struct SimulationSettings final {
  int gridWidth = 128;  // Number of field columns.
  int gridHeight = 128; // Number of field rows.

  float timeStep = 1.0F / 60.0F; // Simulation seconds per solver update.
  float diffusion = 0.0001F;     // Density diffusion coefficient.
  float viscosity = 0.0001F;     // Velocity diffusion coefficient.
  float dissipation = 0.995F;    // Per-step density retention multiplier.
  int pressureIterations = 40;   // Pressure-solver iteration count.

  float brushRadius = 0.04F; // Radius in normalized simulation space.
  float brushDensity = 1.0F; // Density added at the brush center per interaction update.
  float brushForce = 500.0F; // Velocity scale applied to normalized mouse displacement.
};
} // namespace fluid_simulation::simulation
