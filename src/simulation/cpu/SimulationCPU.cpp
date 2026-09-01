#include "simulation/cpu/SimulationCPU.hpp"

#include <cstddef>
#include <stdexcept>

namespace fluid_simulation::simulation::cpu {
namespace {
/**
 * @brief Converts a configured positive grid dimension to the field index type.
 * @param dimension Configured grid dimension to validate.
 * @return Validated dimension as std::size_t.
 * @throws std::invalid_argument If dimension is not positive.
 */
[[nodiscard]] std::size_t CheckedGridDimension(int dimension) {
  if (dimension <= 0) {
    throw std::invalid_argument("Simulation grid dimensions must be positive");
  }

  return static_cast<std::size_t>(dimension);
}
} // namespace

SimulationCPU::SimulationCPU(const SimulationSettings& settings)
    : SimulationCPU(CheckedGridDimension(settings.gridWidth), CheckedGridDimension(settings.gridHeight)) {
}

SimulationCPU::SimulationCPU(std::size_t width, std::size_t height)
    : densityBuffers_(width, height), velocityBuffers_(width, height), pressureBuffers_(width, height), divergence_(width, height) {
}

std::size_t SimulationCPU::Width() const noexcept {
  return densityBuffers_.Source().Width();
}

std::size_t SimulationCPU::Height() const noexcept {
  return densityBuffers_.Source().Height();
}

void SimulationCPU::Reset() noexcept {
  densityBuffers_.Reset();
  velocityBuffers_.Reset();
  pressureBuffers_.Reset();

  divergence_.Clear();
}

ScalarField& SimulationCPU::DensitySource() noexcept {
  return densityBuffers_.Source();
}

const ScalarField& SimulationCPU::DensitySource() const noexcept {
  return densityBuffers_.Source();
}

VectorField& SimulationCPU::VelocitySource() noexcept {
  return velocityBuffers_.Source();
}

const VectorField& SimulationCPU::VelocitySource() const noexcept {
  return velocityBuffers_.Source();
}

ScalarField& SimulationCPU::PressureSource() noexcept {
  return pressureBuffers_.Source();
}

const ScalarField& SimulationCPU::PressureSource() const noexcept {
  return pressureBuffers_.Source();
}

ScalarField& SimulationCPU::Divergence() noexcept {
  return divergence_;
}

const ScalarField& SimulationCPU::Divergence() const noexcept {
  return divergence_;
}

ScalarField& SimulationCPU::DensityDestination() noexcept {
  return densityBuffers_.Destination();
}

const ScalarField& SimulationCPU::DensityDestination() const noexcept {
  return densityBuffers_.Destination();
}

VectorField& SimulationCPU::VelocityDestination() noexcept {
  return velocityBuffers_.Destination();
}

const VectorField& SimulationCPU::VelocityDestination() const noexcept {
  return velocityBuffers_.Destination();
}

ScalarField& SimulationCPU::PressureDestination() noexcept {
  return pressureBuffers_.Destination();
}

const ScalarField& SimulationCPU::PressureDestination() const noexcept {
  return pressureBuffers_.Destination();
}

void SimulationCPU::SwapDensityBuffers() noexcept {
  densityBuffers_.Swap();
}

void SimulationCPU::SwapVelocityBuffers() noexcept {
  velocityBuffers_.Swap();
}

void SimulationCPU::SwapPressureBuffers() noexcept {
  pressureBuffers_.Swap();
}
} // namespace fluid_simulation::simulation::cpu
