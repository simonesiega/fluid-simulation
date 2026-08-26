#pragma once

#include <cstddef>
#include <stdexcept>

namespace fluid_simulation::simulation::cpu {
/**
 * @brief Converts checked two-dimensional grid coordinates to a row-major index.
 * @param x Zero-based column index.
 * @param y Zero-based row index.
 * @param width Number of columns in the grid.
 * @param height Number of rows in the grid.
 * @return Row-major index for the requested coordinates.
 */
[[nodiscard]] inline std::size_t GridIndex(std::size_t x, std::size_t y, std::size_t width, std::size_t height) {
  // Validate each axis before calculating the row-major offset.
  if (x >= width || y >= height) {
    throw std::out_of_range("Grid coordinates are out of bounds");
  }

  return y * width + x;
}
} // namespace fluid_simulation::simulation::cpu
