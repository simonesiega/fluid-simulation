#include "simulation/cpu/VectorField.hpp"

#include "simulation/cpu/GridIndex.hpp"

#include <algorithm>
#include <limits>
#include <stdexcept>
#include <vector>

namespace fluid_simulation::simulation::cpu {
/**
 * @brief Creates a zero-initialized vector field with the requested dimensions.
 * @param width Number of columns in the field.
 * @param height Number of rows in the field.
 */
VectorField::VectorField(std::size_t width, std::size_t height) {
  Resize(width, height);
}

/**
 * @brief Replaces the field with zero-initialized storage of the requested dimensions.
 * @param width Number of columns in the resized field.
 * @param height Number of rows in the resized field.
 * @return Nothing.
 */
void VectorField::Resize(std::size_t width, std::size_t height) {
  // Check multiplication before using the dimensions as an allocation size.
  if (height != 0 && width > std::numeric_limits<std::size_t>::max() / height) {
    throw std::length_error("VectorField dimensions exceed the maximum allocation size");
  }

  std::vector<Vector2f> resizedValues(width * height, Vector2f{0.0F, 0.0F});
  values_.swap(resizedValues);
  width_ = width;
  height_ = height;
}

/**
 * @brief Sets both components of every field value to zero without changing its dimensions.
 * @return Nothing.
 */
void VectorField::Clear() noexcept {
  std::fill(values_.begin(), values_.end(), Vector2f{0.0F, 0.0F});
}

/**
 * @brief Returns mutable access to a value at the requested coordinates.
 * @param x Zero-based column index.
 * @param y Zero-based row index.
 * @return Reference to the selected two-component value.
 */
Vector2f& VectorField::At(std::size_t x, std::size_t y) {
  return values_[GridIndex(x, y, width_, height_)];
}

/**
 * @brief Returns read-only access to a value at the requested coordinates.
 * @param x Zero-based column index.
 * @param y Zero-based row index.
 * @return Const reference to the selected two-component value.
 */
const Vector2f& VectorField::At(std::size_t x, std::size_t y) const {
  return values_[GridIndex(x, y, width_, height_)];
}

/**
 * @brief Returns the field width.
 * @return Number of columns in the field.
 */
std::size_t VectorField::Width() const noexcept {
  return width_;
}

/**
 * @brief Returns the field height.
 * @return Number of rows in the field.
 */
std::size_t VectorField::Height() const noexcept {
  return height_;
}

/**
 * @brief Returns the number of stored two-component values.
 * @return Total number of values in the field.
 */
std::size_t VectorField::Size() const noexcept {
  return values_.size();
}
} // namespace fluid_simulation::simulation::cpu
