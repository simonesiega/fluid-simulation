#pragma once

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <stdexcept>

namespace fluid_simulation::simulation {
/**
 * @brief Identifies one cell in a two-dimensional simulation grid.
 */
struct GridCoordinates {
  std::size_t x = 0;
  std::size_t y = 0;
};

/**
 * @brief Maps normalized simulation coordinates to a valid grid cell.
 * @param normalizedX Horizontal simulation coordinate.
 * @param normalizedY Vertical simulation coordinate.
 * @param width Number of columns in the simulation grid.
 * @param height Number of rows in the simulation grid.
 * @return Grid cell containing the normalized position.
 */
[[nodiscard]] inline GridCoordinates NormalizedToGrid(float normalizedX, float normalizedY, std::size_t width, std::size_t height) {
  if (width == 0 || height == 0) {
    throw std::invalid_argument("Simulation grid dimensions must be positive");
  }

  if (!std::isfinite(normalizedX) || !std::isfinite(normalizedY)) {
    throw std::invalid_argument("Normalized coordinates must be finite");
  }

  const float clampedX = std::clamp(normalizedX, 0.0F, 1.0F);
  const float clampedY = std::clamp(normalizedY, 0.0F, 1.0F);

  return {
    std::min(static_cast<std::size_t>(clampedX * static_cast<float>(width)), width - 1),
    std::min(static_cast<std::size_t>(clampedY * static_cast<float>(height)), height - 1),
  };
}
} // namespace fluid_simulation::simulation
