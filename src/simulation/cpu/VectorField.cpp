#include "simulation/cpu/VectorField.hpp"

#include "simulation/cpu/GridIndex.hpp"

#include <algorithm>
#include <cmath>
#include <limits>
#include <stdexcept>
#include <vector>

namespace fluid_simulation::simulation::cpu {
VectorField::VectorField(std::size_t width, std::size_t height) {
  Resize(width, height);
}

void VectorField::Resize(std::size_t width, std::size_t height) {
  // Validate the allocation size before multiplying the dimensions.
  if (height != 0 && width > std::numeric_limits<std::size_t>::max() / height) {
    throw std::length_error("VectorField dimensions exceed the maximum allocation size");
  }

  std::vector<Vector2f> resizedValues(width * height, Vector2f{0.0F, 0.0F});
  values_.swap(resizedValues);

  width_ = width;
  height_ = height;
}

void VectorField::Clear() noexcept {
  std::fill(values_.begin(), values_.end(), Vector2f{0.0F, 0.0F});
}

Vector2f& VectorField::At(std::size_t x, std::size_t y) {
  return values_[GridIndex(x, y, width_, height_)];
}

const Vector2f& VectorField::At(std::size_t x, std::size_t y) const {
  return values_[GridIndex(x, y, width_, height_)];
}

Vector2f VectorField::SampleBilinear(float x, float y) const {
  if (width_ == 0 || height_ == 0) {
    throw std::out_of_range("Cannot sample an empty VectorField");
  }

  if (!std::isfinite(x) || !std::isfinite(y)) {
    throw std::invalid_argument("VectorField sample coordinates must be finite");
  }

  const float clampedX = std::clamp(x, 0.0F, static_cast<float>(width_ - 1));
  const float clampedY = std::clamp(y, 0.0F, static_cast<float>(height_ - 1));

  // Guard against large dimensions whose last index is not exactly representable as float.
  const std::size_t x0 = std::min(static_cast<std::size_t>(clampedX), width_ - 1);
  const std::size_t y0 = std::min(static_cast<std::size_t>(clampedY), height_ - 1);
  const std::size_t x1 = std::min(x0 + 1, width_ - 1);
  const std::size_t y1 = std::min(y0 + 1, height_ - 1);

  const float tx = clampedX - static_cast<float>(x0);
  const float ty = clampedY - static_cast<float>(y0);

  // Coordinates are already validated, so avoid redundant checked accesses.
  const std::size_t topRow = y0 * width_;
  const std::size_t bottomRow = y1 * width_;

  const Vector2f& v00 = values_[topRow + x0];
  const Vector2f& v10 = values_[topRow + x1];
  const Vector2f& v01 = values_[bottomRow + x0];
  const Vector2f& v11 = values_[bottomRow + x1];

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

std::size_t VectorField::Width() const noexcept {
  return width_;
}

std::size_t VectorField::Height() const noexcept {
  return height_;
}

std::size_t VectorField::Size() const noexcept {
  return values_.size();
}
} // namespace fluid_simulation::simulation::cpu
