#include "simulation/cpu/ScalarField.hpp"

#include <algorithm>
#include <limits>
#include <stdexcept>
#include <vector>

namespace fluid_simulation::simulation::cpu {
ScalarField::ScalarField(std::size_t width, std::size_t height) {
  Resize(width, height);
}

void ScalarField::Resize(std::size_t width, std::size_t height) {
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
  if (x >= width_ || y >= height_) {
    throw std::out_of_range("ScalarField coordinates are out of bounds");
  }

  return values_[y * width_ + x];
}

const float& ScalarField::At(std::size_t x, std::size_t y) const {
  if (x >= width_ || y >= height_) {
    throw std::out_of_range("ScalarField coordinates are out of bounds");
  }

  return values_[y * width_ + x];
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
