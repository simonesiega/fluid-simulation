#include "simulation/cpu/SimulationCPU.hpp"

#include <cstddef>
#include <stdexcept>

namespace fluid_simulation::simulation::cpu {
namespace {
/**
 * @brief Converts a positive configured grid dimension to the field index type.
 * @param dimension Configured grid dimension.
 * @return Validated grid dimension as std::size_t.
 */
[[nodiscard]] std::size_t CheckedGridDimension(int dimension) {
  if (dimension <= 0) {
    throw std::invalid_argument("Simulation grid dimensions must be positive");
  }

  return static_cast<std::size_t>(dimension);
}
} // namespace

/**
 * @brief Creates zero-initialized CPU fields using one shared grid size.
 * @param settings Simulation settings containing the grid dimensions.
 */
SimulationCPU::SimulationCPU(const SimulationSettings& settings)
    : SimulationCPU(CheckedGridDimension(settings.gridWidth), CheckedGridDimension(settings.gridHeight)) {
}

/**
 * @brief Initializes every owned field with validated grid dimensions.
 * @param width Number of columns shared by all fields.
 * @param height Number of rows shared by all fields.
 */
SimulationCPU::SimulationCPU(std::size_t width, std::size_t height)
    : density_(width, height), velocity_(width, height), pressure_(width, height), divergence_(width, height),
      densityScratch_(width, height), velocityScratch_(width, height), pressureScratch_(width, height) {
}

/**
 * @brief Returns the shared simulation grid width.
 * @return Number of columns in every simulation field.
 */
std::size_t SimulationCPU::Width() const noexcept {
  return density_.Width();
}

/**
 * @brief Returns the shared simulation grid height.
 * @return Number of rows in every simulation field.
 */
std::size_t SimulationCPU::Height() const noexcept {
  return density_.Height();
}

/**
 * @brief Returns mutable access to the density field.
 * @return Density field.
 */
ScalarField& SimulationCPU::Density() noexcept {
  return density_;
}

/**
 * @brief Returns read-only access to the density field.
 * @return Density field.
 */
const ScalarField& SimulationCPU::Density() const noexcept {
  return density_;
}

/**
 * @brief Returns mutable access to the velocity field.
 * @return Velocity field.
 */
VectorField& SimulationCPU::Velocity() noexcept {
  return velocity_;
}

/**
 * @brief Returns read-only access to the velocity field.
 * @return Velocity field.
 */
const VectorField& SimulationCPU::Velocity() const noexcept {
  return velocity_;
}

/**
 * @brief Returns mutable access to the pressure field.
 * @return Pressure field.
 */
ScalarField& SimulationCPU::Pressure() noexcept {
  return pressure_;
}

/**
 * @brief Returns read-only access to the pressure field.
 * @return Pressure field.
 */
const ScalarField& SimulationCPU::Pressure() const noexcept {
  return pressure_;
}

/**
 * @brief Returns mutable access to the divergence field.
 * @return Divergence field.
 */
ScalarField& SimulationCPU::Divergence() noexcept {
  return divergence_;
}

/**
 * @brief Returns read-only access to the divergence field.
 * @return Divergence field.
 */
const ScalarField& SimulationCPU::Divergence() const noexcept {
  return divergence_;
}

/**
 * @brief Returns mutable access to the density scratch field.
 * @return Density scratch field.
 */
ScalarField& SimulationCPU::DensityScratch() noexcept {
  return densityScratch_;
}

/**
 * @brief Returns read-only access to the density scratch field.
 * @return Density scratch field.
 */
const ScalarField& SimulationCPU::DensityScratch() const noexcept {
  return densityScratch_;
}

/**
 * @brief Returns mutable access to the velocity scratch field.
 * @return Velocity scratch field.
 */
VectorField& SimulationCPU::VelocityScratch() noexcept {
  return velocityScratch_;
}

/**
 * @brief Returns read-only access to the velocity scratch field.
 * @return Velocity scratch field.
 */
const VectorField& SimulationCPU::VelocityScratch() const noexcept {
  return velocityScratch_;
}

/**
 * @brief Returns mutable access to the pressure scratch field.
 * @return Pressure scratch field.
 */
ScalarField& SimulationCPU::PressureScratch() noexcept {
  return pressureScratch_;
}

/**
 * @brief Returns read-only access to the pressure scratch field.
 * @return Pressure scratch field.
 */
const ScalarField& SimulationCPU::PressureScratch() const noexcept {
  return pressureScratch_;
}
} // namespace fluid_simulation::simulation::cpu
