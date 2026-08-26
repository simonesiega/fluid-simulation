#include "simulation/cpu/VectorField.hpp"

#include <algorithm>
#include <limits>
#include <stdexcept>
#include <vector>

namespace fluid_simulation::simulation::cpu {
VectorField::VectorField(std::size_t width, std::size_t height) {
  Resize(width, height);
}

void VectorField::Resize(std::size_t width, std::size_t height) {
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
  if (x >= width_ || y >= height_) {
    throw std::out_of_range("VectorField coordinates are out of bounds");
  }

  return values_[y * width_ + x];
}

const Vector2f& VectorField::At(std::size_t x, std::size_t y) const {
  if (x >= width_ || y >= height_) {
    throw std::out_of_range("VectorField coordinates are out of bounds");
  }

  return values_[y * width_ + x];
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
