#pragma once

#include "simulation/SimulationSettings.hpp"
#include "simulation/cpu/PingPongField.hpp"
#include "simulation/cpu/ScalarField.hpp"
#include "simulation/cpu/VectorField.hpp"

#include <cstddef>

namespace fluid_simulation::simulation::cpu {
/**
 * @brief Owns the CPU fields and source/destination buffers used by the fluid simulation.
 *
 * All owned fields are constructed with one shared grid size. Density, velocity,
 * and pressure use independent ping-pong pairs; divergence is single-buffered.
 * Callers may modify field values but must not resize returned fields independently.
 */
class SimulationCPU final {
public:
  /**
   * @brief Creates zero-initialized CPU fields using one shared grid size.
   * @param settings Simulation settings containing the grid dimensions.
   * @throws std::invalid_argument If either configured grid dimension is not positive.
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
   * @brief Clears every CPU field and restores the original ping-pong buffer roles.
   */
  void Reset() noexcept;

  /**
   * @brief Returns mutable access to the current density source field.
   * @return Current density source field.
   */
  [[nodiscard]] ScalarField& DensitySource() noexcept;

  /**
   * @brief Returns read-only access to the current density source field.
   * @return Current density source field.
   */
  [[nodiscard]] const ScalarField& DensitySource() const noexcept;

  /**
   * @brief Returns mutable access to the current velocity source field.
   * @return Current velocity source field.
   */
  [[nodiscard]] VectorField& VelocitySource() noexcept;

  /**
   * @brief Returns read-only access to the current velocity source field.
   * @return Current velocity source field.
   */
  [[nodiscard]] const VectorField& VelocitySource() const noexcept;

  /**
   * @brief Returns mutable access to the current pressure source field.
   * @return Current pressure source field.
   */
  [[nodiscard]] ScalarField& PressureSource() noexcept;

  /**
   * @brief Returns read-only access to the current pressure source field.
   * @return Current pressure source field.
   */
  [[nodiscard]] const ScalarField& PressureSource() const noexcept;

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
   * @brief Returns mutable access to the current density destination field.
   * @return Current density destination field.
   */
  [[nodiscard]] ScalarField& DensityDestination() noexcept;

  /**
   * @brief Returns read-only access to the current density destination field.
   * @return Current density destination field.
   */
  [[nodiscard]] const ScalarField& DensityDestination() const noexcept;

  /**
   * @brief Returns mutable access to the current velocity destination field.
   * @return Current velocity destination field.
   */
  [[nodiscard]] VectorField& VelocityDestination() noexcept;

  /**
   * @brief Returns read-only access to the current velocity destination field.
   * @return Current velocity destination field.
   */
  [[nodiscard]] const VectorField& VelocityDestination() const noexcept;

  /**
   * @brief Returns mutable access to the current pressure destination field.
   * @return Current pressure destination field.
   */
  [[nodiscard]] ScalarField& PressureDestination() noexcept;

  /**
   * @brief Returns read-only access to the current pressure destination field.
   * @return Current pressure destination field.
   */
  [[nodiscard]] const ScalarField& PressureDestination() const noexcept;

  /**
   * @brief Exchanges the logical density source and destination fields in constant time.
   */
  void SwapDensityBuffers() noexcept;

  /**
   * @brief Exchanges the logical velocity source and destination fields in constant time.
   */
  void SwapVelocityBuffers() noexcept;

  /**
   * @brief Exchanges the logical pressure source and destination fields in constant time.
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
