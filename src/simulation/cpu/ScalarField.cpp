#include "simulation/cpu/ScalarField.hpp"

#include "simulation/cpu/GridIndex.hpp"

#include <algorithm>
#include <cmath>
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
 * @brief Samples the field at fractional grid coordinates using bilinear interpolation.
 * @param x Horizontal grid-space coordinate, clamped to the field boundary.
 * @param y Vertical grid-space coordinate, clamped to the field boundary.
 * @return Interpolated scalar value.
 */
float ScalarField::SampleBilinear(float x, float y) const {
  if (width_ == 0 || height_ == 0) {
    throw std::out_of_range("Cannot sample an empty ScalarField");
  }

  if (!std::isfinite(x) || !std::isfinite(y)) {
    throw std::invalid_argument("ScalarField sample coordinates must be finite");
  }

  const float clampedX = std::clamp(x, 0.0F, static_cast<float>(width_ - 1));
  const float clampedY = std::clamp(y, 0.0F, static_cast<float>(height_ - 1));
  const std::size_t x0 = static_cast<std::size_t>(std::floor(clampedX));
  const std::size_t y0 = static_cast<std::size_t>(std::floor(clampedY));
  const std::size_t x1 = std::min(x0 + 1, width_ - 1);
  const std::size_t y1 = std::min(y0 + 1, height_ - 1);
  const float tx = clampedX - static_cast<float>(x0);
  const float ty = clampedY - static_cast<float>(y0);

  const float v00 = At(x0, y0);
  const float v10 = At(x1, y0);
  const float v01 = At(x0, y1);
  const float v11 = At(x1, y1);
  const float top = v00 * (1.0F - tx) + v10 * tx;
  const float bottom = v01 * (1.0F - tx) + v11 * tx;

  return top * (1.0F - ty) + bottom * ty;
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
