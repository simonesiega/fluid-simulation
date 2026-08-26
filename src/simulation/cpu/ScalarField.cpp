#include "simulation/cpu/ScalarField.hpp"

#include "simulation/cpu/GridIndex.hpp"

#include <algorithm>
#include <limits>
#include <stdexcept>
#include <vector>

namespace fluid_simulation::simulation::cpu {
/**
 * @brief Creates a zero-initialized scalar field with the requested dimensions.
 * @param width Number of columns in the field.
 * @param height Number of rows in the field.
 */
ScalarField::ScalarField(std::size_t width, std::size_t height) {
  Resize(width, height);
}

/**
 * @brief Replaces the field with zero-initialized storage of the requested dimensions.
 * @param width Number of columns in the resized field.
 * @param height Number of rows in the resized field.
 * @return Nothing.
 */
void ScalarField::Resize(std::size_t width, std::size_t height) {
  // Check multiplication before using the dimensions as an allocation size.
  if (height != 0 && width > std::numeric_limits<std::size_t>::max() / height) {
    throw std::length_error("ScalarField dimensions exceed the maximum allocation size");
  }

  std::vector<float> resizedValues(width * height, 0.0F);
  values_.swap(resizedValues);
  width_ = width;
  height_ = height;
}

/**
 * @brief Sets every field value to zero without changing its dimensions.
 * @return Nothing.
 */
void ScalarField::Clear() noexcept {
  std::fill(values_.begin(), values_.end(), 0.0F);
}

/**
 * @brief Returns mutable access to a value at the requested coordinates.
 * @param x Zero-based column index.
 * @param y Zero-based row index.
 * @return Reference to the selected scalar value.
 */
float& ScalarField::At(std::size_t x, std::size_t y) {
  return values_[GridIndex(x, y, width_, height_)];
}

/**
 * @brief Returns read-only access to a value at the requested coordinates.
 * @param x Zero-based column index.
 * @param y Zero-based row index.
 * @return Const reference to the selected scalar value.
 */
const float& ScalarField::At(std::size_t x, std::size_t y) const {
  return values_[GridIndex(x, y, width_, height_)];
}

/**
 * @brief Returns the field width.
 * @return Number of columns in the field.
 */
std::size_t ScalarField::Width() const noexcept {
  return width_;
}

/**
 * @brief Returns the field height.
 * @return Number of rows in the field.
 */
std::size_t ScalarField::Height() const noexcept {
  return height_;
}

/**
 * @brief Returns the number of stored scalar values.
 * @return Total number of values in the field.
 */
std::size_t ScalarField::Size() const noexcept {
  return values_.size();
}
} // namespace fluid_simulation::simulation::cpu
