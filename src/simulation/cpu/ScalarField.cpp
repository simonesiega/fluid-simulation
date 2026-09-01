#include "simulation/cpu/ScalarField.hpp"

#include "simulation/cpu/GridIndex.hpp"

#include <algorithm>
#include <cmath>
#include <limits>
#include <stdexcept>
#include <vector>

namespace fluid_simulation::simulation::cpu {
ScalarField::ScalarField(std::size_t width, std::size_t height) {
  Resize(width, height);
}

void ScalarField::Resize(std::size_t width, std::size_t height) {
  // Validate the allocation size before multiplying the dimensions.
  if (height != 0 && width > std::numeric_limits<std::size_t>::max() / height) {
    throw std::length_error("ScalarField dimensions exceed the maximum allocation size");
  }

  std::vector<float> resizedValues(width * height, 0.0F);
  values_.swap(resizedValues);

  width_ = width;
  height_ = height;
}

void ScalarField::Clear() noexcept {
  std::fill(values_.begin(), values_.end(), 0.0F);
}

float& ScalarField::At(std::size_t x, std::size_t y) {
  return values_[GridIndex(x, y, width_, height_)];
}

const float& ScalarField::At(std::size_t x, std::size_t y) const {
  return values_[GridIndex(x, y, width_, height_)];
}

float ScalarField::SampleBilinear(float x, float y) const {
  if (width_ == 0 || height_ == 0) {
    throw std::out_of_range("Cannot sample an empty ScalarField");
  }

  if (!std::isfinite(x) || !std::isfinite(y)) {
    throw std::invalid_argument("ScalarField sample coordinates must be finite");
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

  const float v00 = values_[topRow + x0];
  const float v10 = values_[topRow + x1];
  const float v01 = values_[bottomRow + x0];
  const float v11 = values_[bottomRow + x1];

  const float top = v00 * (1.0F - tx) + v10 * tx;
  const float bottom = v01 * (1.0F - tx) + v11 * tx;

  return top * (1.0F - ty) + bottom * ty;
}

std::size_t ScalarField::Width() const noexcept {
  return width_;
}

std::size_t ScalarField::Height() const noexcept {
  return height_;
}

std::size_t ScalarField::Size() const noexcept {
  return values_.size();
}
} // namespace fluid_simulation::simulation::cpu
