#pragma once

#include <cstddef>
#include <vector>

namespace fluid_simulation::simulation::cpu {
struct Vector2f {
  float x = 0.0F;
  float y = 0.0F;
};

class VectorField final {
public:
  /**
   * @brief Creates an empty vector field.
   */
  VectorField() = default;

  /**
   * @brief Creates a zero-initialized vector field with the requested dimensions.
   * @param width Number of columns in the field.
   * @param height Number of rows in the field.
   */
  VectorField(std::size_t width, std::size_t height);

  /**
   * @brief Replaces the field with zero-initialized storage of the requested dimensions.
   * @param width Number of columns in the resized field.
   * @param height Number of rows in the resized field.
   * @return Nothing.
   */
  void Resize(std::size_t width, std::size_t height);

  /**
   * @brief Sets both components of every field value to zero without changing its dimensions.
   * @return Nothing.
   */
  void Clear() noexcept;

  /**
   * @brief Returns mutable access to a value at the requested coordinates.
   * @param x Zero-based column index.
   * @param y Zero-based row index.
   * @return Reference to the selected two-component value.
   */
  [[nodiscard]] Vector2f& At(std::size_t x, std::size_t y);

  /**
   * @brief Returns read-only access to a value at the requested coordinates.
   * @param x Zero-based column index.
   * @param y Zero-based row index.
   * @return Const reference to the selected two-component value.
   */
  [[nodiscard]] const Vector2f& At(std::size_t x, std::size_t y) const;

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
   * @brief Returns the number of stored two-component values.
   * @return Total number of values in the field.
   */
  [[nodiscard]] std::size_t Size() const noexcept;

private:
  std::size_t width_ = 0;
  std::size_t height_ = 0;
  std::vector<Vector2f> values_;
};
} // namespace fluid_simulation::simulation::cpu
