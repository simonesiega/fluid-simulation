#pragma once

namespace fluid_simulation::simulation {
struct SimulationSettings final {
  int gridWidth = 128;
  int gridHeight = 128;
  float timeStep = 1.0F / 60.0F;
  float diffusion = 0.0001F;
  float viscosity = 0.0001F;
  float dissipation = 0.995F;
  int pressureIterations = 40;
  float brushRadius = 0.04F; // Normalized simulation-space radius.
  float brushDensity = 1.0F;
  float brushForce = 500.0F;
};
} // namespace fluid_simulation::simulation
