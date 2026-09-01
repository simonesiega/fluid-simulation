#pragma once

#include "simulation/GridCoordinates.hpp"
#include "simulation/SimulationSettings.hpp"
#include "simulation/cpu/SimulationCPU.hpp"

namespace fluid_simulation::simulation::cpu {
/**
 * @brief Injects density and mouse-drag velocity using a circular linear-falloff brush.
 *
 * A finite radius is clamped to [0, 1]; a non-finite radius disables injection.
 * Density accumulates for a positive finite density setting. Velocity additionally
 * requires a positive finite force, finite mouse movement, and a finite scaled result;
 * zero mouse movement therefore injects density without changing velocity.
 *
 * @param simulation CPU simulation whose source fields are modified.
 * @param settings Simulation settings controlling brush radius, density, and force.
 * @param center Grid cell at the center of the brush.
 * @param normalizedDeltaX Horizontal mouse movement in normalized simulation space.
 * @param normalizedDeltaY Vertical mouse movement in normalized simulation space.
 * @throws std::out_of_range If the brush center is outside the simulation grid.
 */
void InjectDensityAndVelocity(SimulationCPU& simulation,
                              const SimulationSettings& settings,
                              const GridCoordinates& center,
                              float normalizedDeltaX,
                              float normalizedDeltaY);
} // namespace fluid_simulation::simulation::cpu
