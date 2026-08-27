#pragma once

#include "simulation/SimulationSettings.hpp"
#include "simulation/cpu/PingPongField.hpp"
#include "simulation/cpu/ScalarField.hpp"
#include "simulation/cpu/VectorField.hpp"

#include <cstddef>

namespace fluid_simulation::simulation::cpu {
/**
 * @brief Owns the CPU fields and source/destination buffers used by the fluid simulation.
 */
class SimulationCPU final {
public:
  /**
   * @brief Creates zero-initialized CPU fields using one shared grid size.
   * @param settings Simulation settings containing the grid dimensions.
   */
  explicit SimulationCPU(const SimulationSettings& settings);

  /**
   * @brief Returns the shared simulation grid width.
   * @return Number of columns in every simulation field.
   */
  [[nodiscard]] std::size_t Width() const noexcept;

  /**
   * @brief Returns the shared simulation grid height.
   * @return Number of rows in every simulation field.
   */
  [[nodiscard]] std::size_t Height() const noexcept;

  /**
   * @brief Returns mutable access to the density field.
   * @return Density field.
   */
  [[nodiscard]] ScalarField& Density() noexcept;

  /**
   * @brief Returns read-only access to the density field.
   * @return Density field.
   */
  [[nodiscard]] const ScalarField& Density() const noexcept;

  /**
   * @brief Returns mutable access to the velocity field.
   * @return Velocity field.
   */
  [[nodiscard]] VectorField& Velocity() noexcept;

  /**
   * @brief Returns read-only access to the velocity field.
   * @return Velocity field.
   */
  [[nodiscard]] const VectorField& Velocity() const noexcept;

  /**
   * @brief Returns mutable access to the pressure field.
   * @return Pressure field.
   */
  [[nodiscard]] ScalarField& Pressure() noexcept;

  /**
   * @brief Returns read-only access to the pressure field.
   * @return Pressure field.
   */
  [[nodiscard]] const ScalarField& Pressure() const noexcept;

  /**
   * @brief Returns mutable access to the divergence field.
   * @return Divergence field.
   */
  [[nodiscard]] ScalarField& Divergence() noexcept;

  /**
   * @brief Returns read-only access to the divergence field.
   * @return Divergence field.
   */
  [[nodiscard]] const ScalarField& Divergence() const noexcept;

  /**
   * @brief Returns mutable access to the density scratch field.
   * @return Density scratch field.
   */
  [[nodiscard]] ScalarField& DensityScratch() noexcept;

  /**
   * @brief Returns read-only access to the density scratch field.
   * @return Density scratch field.
   */
  [[nodiscard]] const ScalarField& DensityScratch() const noexcept;

  /**
   * @brief Returns mutable access to the velocity scratch field.
   * @return Velocity scratch field.
   */
  [[nodiscard]] VectorField& VelocityScratch() noexcept;

  /**
   * @brief Returns read-only access to the velocity scratch field.
   * @return Velocity scratch field.
   */
  [[nodiscard]] const VectorField& VelocityScratch() const noexcept;

  /**
   * @brief Returns mutable access to the pressure scratch field.
   * @return Pressure scratch field.
   */
  [[nodiscard]] ScalarField& PressureScratch() noexcept;

  /**
   * @brief Returns read-only access to the pressure scratch field.
   * @return Pressure scratch field.
   */
  [[nodiscard]] const ScalarField& PressureScratch() const noexcept;

  /**
   * @brief Exchanges the logical density source and destination fields.
   * @return Nothing.
   */
  void SwapDensityBuffers() noexcept;

  /**
   * @brief Exchanges the logical velocity source and destination fields.
   * @return Nothing.
   */
  void SwapVelocityBuffers() noexcept;

  /**
   * @brief Exchanges the logical pressure source and destination fields.
   * @return Nothing.
   */
  void SwapPressureBuffers() noexcept;

private:
  /**
   * @brief Initializes every owned field with validated grid dimensions.
   * @param width Number of columns shared by all fields.
   * @param height Number of rows shared by all fields.
   */
  SimulationCPU(std::size_t width, std::size_t height);

  PingPongField<ScalarField> densityBuffers_;
  PingPongField<VectorField> velocityBuffers_;
  PingPongField<ScalarField> pressureBuffers_;
  ScalarField divergence_;
};
} // namespace fluid_simulation::simulation::cpu
