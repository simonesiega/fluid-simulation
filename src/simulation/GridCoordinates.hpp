#pragma once

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <stdexcept>

namespace fluid_simulation::simulation {
/**
 * @brief Identifies one cell in a two-dimensional simulation grid.
 *
 * The origin is the top-left cell. x increases rightward and y increases downward,
 * matching normalized viewport, screen, field, and velocity coordinates.
 */
struct GridCoordinates {
  std::size_t x = 0;
  std::size_t y = 0;
};

/**
 * @brief Maps normalized simulation coordinates to a valid grid cell.
 *
 * Values outside [0, 1] are clamped. Zero maps to the first cell and one maps
 * to the last cell on each axis.
 *
 * @param normalizedX Horizontal simulation coordinate increasing rightward.
 * @param normalizedY Vertical simulation coordinate increasing downward.
 * @param width Number of columns in the simulation grid.
 * @param height Number of rows in the simulation grid.
 * @return Grid cell containing the normalized position.
 * @throws std::invalid_argument If either dimension is zero or either coordinate is not finite.
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

  std::size_t gridX = width - 1;
  std::size_t gridY = height - 1;
  if (clampedX < 1.0F) {
    gridX = std::min(static_cast<std::size_t>(clampedX * static_cast<float>(width)), width - 1);
  }
  if (clampedY < 1.0F) {
    gridY = std::min(static_cast<std::size_t>(clampedY * static_cast<float>(height)), height - 1);
  }

  return {gridX, gridY};
}
} // namespace fluid_simulation::simulation
