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
    : densityBuffers_(width, height), velocityBuffers_(width, height), pressureBuffers_(width, height), divergence_(width, height) {
}

/**
 * @brief Returns the shared simulation grid width.
 * @return Number of columns in every simulation field.
 */
std::size_t SimulationCPU::Width() const noexcept {
  return densityBuffers_.Source().Width();
}

/**
 * @brief Returns the shared simulation grid height.
 * @return Number of rows in every simulation field.
 */
std::size_t SimulationCPU::Height() const noexcept {
  return densityBuffers_.Source().Height();
}

/**
 * @brief Returns mutable access to the density field.
 * @return Density field.
 */
ScalarField& SimulationCPU::Density() noexcept {
  return densityBuffers_.Source();
}

/**
 * @brief Returns read-only access to the density field.
 * @return Density field.
 */
const ScalarField& SimulationCPU::Density() const noexcept {
  return densityBuffers_.Source();
}

/**
 * @brief Returns mutable access to the velocity field.
 * @return Velocity field.
 */
VectorField& SimulationCPU::Velocity() noexcept {
  return velocityBuffers_.Source();
}

/**
 * @brief Returns read-only access to the velocity field.
 * @return Velocity field.
 */
const VectorField& SimulationCPU::Velocity() const noexcept {
  return velocityBuffers_.Source();
}

/**
 * @brief Returns mutable access to the pressure field.
 * @return Pressure field.
 */
ScalarField& SimulationCPU::Pressure() noexcept {
  return pressureBuffers_.Source();
}

/**
 * @brief Returns read-only access to the pressure field.
 * @return Pressure field.
 */
const ScalarField& SimulationCPU::Pressure() const noexcept {
  return pressureBuffers_.Source();
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
  return densityBuffers_.Destination();
}

/**
 * @brief Returns read-only access to the density scratch field.
 * @return Density scratch field.
 */
const ScalarField& SimulationCPU::DensityScratch() const noexcept {
  return densityBuffers_.Destination();
}

/**
 * @brief Returns mutable access to the velocity scratch field.
 * @return Velocity scratch field.
 */
VectorField& SimulationCPU::VelocityScratch() noexcept {
  return velocityBuffers_.Destination();
}

/**
 * @brief Returns read-only access to the velocity scratch field.
 * @return Velocity scratch field.
 */
const VectorField& SimulationCPU::VelocityScratch() const noexcept {
  return velocityBuffers_.Destination();
}

/**
 * @brief Returns mutable access to the pressure scratch field.
 * @return Pressure scratch field.
 */
ScalarField& SimulationCPU::PressureScratch() noexcept {
  return pressureBuffers_.Destination();
}

/**
 * @brief Returns read-only access to the pressure scratch field.
 * @return Pressure scratch field.
 */
const ScalarField& SimulationCPU::PressureScratch() const noexcept {
  return pressureBuffers_.Destination();
}

/**
 * @brief Exchanges the logical density source and destination fields.
 * @return Nothing.
 */
void SimulationCPU::SwapDensityBuffers() noexcept {
  densityBuffers_.Swap();
}

/**
 * @brief Exchanges the logical velocity source and destination fields.
 * @return Nothing.
 */
void SimulationCPU::SwapVelocityBuffers() noexcept {
  velocityBuffers_.Swap();
}

/**
 * @brief Exchanges the logical pressure source and destination fields.
 * @return Nothing.
 */
void SimulationCPU::SwapPressureBuffers() noexcept {
  pressureBuffers_.Swap();
}
} // namespace fluid_simulation::simulation::cpu
