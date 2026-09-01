#pragma once

#include <cstddef>
#include <vector>

namespace fluid_simulation::simulation::cpu {
/**
 * @brief Owns a contiguous, row-major two-dimensional field of scalar values.
 *
 * Coordinates use a top-left origin with x increasing rightward and y increasing downward.
 */
class ScalarField final {
public:
  /**
   * @brief Creates an empty scalar field.
   */
  ScalarField() = default;

  /**
   * @brief Creates a zero-initialized scalar field with the requested dimensions.
   *
   * If either dimension is zero, the field has empty storage while retaining both requested dimensions.
   *
   * @param width Number of columns in the field.
   * @param height Number of rows in the field.
   * @throws std::length_error If the requested dimensions exceed the maximum allocation size.
   */
  ScalarField(std::size_t width, std::size_t height);

  /**
   * @brief Replaces the field with zero-initialized storage of the requested dimensions.
   *
   * Existing element references are invalidated. If allocation fails, the field is unchanged.
   * A zero dimension produces empty storage while preserving both requested dimensions.
   *
   * @param width Number of columns in the resized field.
   * @param height Number of rows in the resized field.
   * @throws std::length_error If the requested dimensions exceed the maximum allocation size.
   */
  void Resize(std::size_t width, std::size_t height);

  /**
   * @brief Sets every field value to zero without changing its dimensions.
   */
  void Clear() noexcept;

  /**
   * @brief Returns mutable access to a value at the requested coordinates.
   * @param x Zero-based column index.
   * @param y Zero-based row index.
   * @return Reference to the selected scalar value.
   * @throws std::out_of_range If the coordinates are outside the field.
   */
  [[nodiscard]] float& At(std::size_t x, std::size_t y);

  /**
   * @brief Returns read-only access to a value at the requested coordinates.
   * @param x Zero-based column index.
   * @param y Zero-based row index.
   * @return Const reference to the selected scalar value.
   * @throws std::out_of_range If the coordinates are outside the field.
   */
  [[nodiscard]] const float& At(std::size_t x, std::size_t y) const;

  /**
   * @brief Samples the field at fractional grid coordinates using bilinear interpolation.
   * @param x Horizontal grid-space coordinate, clamped to the field boundary.
   * @param y Vertical grid-space coordinate, clamped to the field boundary.
   * @return Interpolated scalar value.
   * @throws std::out_of_range If the field is empty.
   * @throws std::invalid_argument If either coordinate is not finite.
   */
  [[nodiscard]] float SampleBilinear(float x, float y) const;

  /**
   * @brief Returns the field width.
   * @return Number of columns in the field.
   */
  [[nodiscard]] std::size_t Width() const noexcept;

  /**
   * @brief Returns the field height.
   * @return Number of rows in the field.
   */
  [[nodiscard]] std::size_t Height() const noexcept;

  /**
   * @brief Returns the number of stored scalar values.
   * @return Total number of values in the field.
   */
  [[nodiscard]] std::size_t Size() const noexcept;

private:
  std::size_t width_ = 0;
  std::size_t height_ = 0;
  std::vector<float> values_;
};
} // namespace fluid_simulation::simulation::cpu
