#include "simulation/cpu/VectorField.hpp"

#include "simulation/cpu/GridIndex.hpp"

#include <algorithm>
#include <cmath>
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
 * @brief Samples the field at fractional grid coordinates using bilinear interpolation.
 * @param x Horizontal grid-space coordinate, clamped to the field boundary.
 * @param y Vertical grid-space coordinate, clamped to the field boundary.
 * @return Interpolated two-component value.
 */
Vector2f VectorField::SampleBilinear(float x, float y) const {
  if (width_ == 0 || height_ == 0) {
    throw std::out_of_range("Cannot sample an empty VectorField");
  }

  if (!std::isfinite(x) || !std::isfinite(y)) {
    throw std::invalid_argument("VectorField sample coordinates must be finite");
  }

  const float clampedX = std::clamp(x, 0.0F, static_cast<float>(width_ - 1));
  const float clampedY = std::clamp(y, 0.0F, static_cast<float>(height_ - 1));
  const std::size_t x0 = static_cast<std::size_t>(std::floor(clampedX));
  const std::size_t y0 = static_cast<std::size_t>(std::floor(clampedY));
  const std::size_t x1 = std::min(x0 + 1, width_ - 1);
  const std::size_t y1 = std::min(y0 + 1, height_ - 1);
  const float tx = clampedX - static_cast<float>(x0);
  const float ty = clampedY - static_cast<float>(y0);

  const Vector2f& v00 = At(x0, y0);
  const Vector2f& v10 = At(x1, y0);
  const Vector2f& v01 = At(x0, y1);
  const Vector2f& v11 = At(x1, y1);
  const Vector2f top = {
    v00.x * (1.0F - tx) + v10.x * tx,
    v00.y * (1.0F - tx) + v10.y * tx,
  };
  const Vector2f bottom = {
    v01.x * (1.0F - tx) + v11.x * tx,
    v01.y * (1.0F - tx) + v11.y * tx,
  };

  return {
    top.x * (1.0F - ty) + bottom.x * ty,
    top.y * (1.0F - ty) + bottom.y * ty,
  };
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
