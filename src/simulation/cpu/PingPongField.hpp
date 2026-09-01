#pragma once

#include <cstddef>

namespace fluid_simulation::simulation::cpu {
/**
 * @brief Owns two equally sized fields with interchangeable source and destination roles.
 *
 * Callers may mutate field values but must not resize either field through a returned
 * reference, because that would violate the matching-dimensions invariant.
 *
 * @tparam Field Field storage type constructed from width and height and providing noexcept Clear().
 */
template <typename Field> class PingPongField final {
public:
  /**
   * @brief Creates zero-initialized source and destination fields with matching dimensions.
   * @param width Number of columns in each field.
   * @param height Number of rows in each field.
   */
  PingPongField(std::size_t width, std::size_t height) : first_(width, height), second_(width, height) {
  }

  /**
   * @brief Returns mutable access to the current source field.
   * @return Current logical source field.
   */
  [[nodiscard]] Field& Source() noexcept {
    return sourceIsFirst_ ? first_ : second_;
  }

  /**
   * @brief Returns read-only access to the current source field.
   * @return Current logical source field.
   */
  [[nodiscard]] const Field& Source() const noexcept {
    return sourceIsFirst_ ? first_ : second_;
  }

  /**
   * @brief Returns mutable access to the current destination field.
   * @return Current logical destination field.
   */
  [[nodiscard]] Field& Destination() noexcept {
    return sourceIsFirst_ ? second_ : first_;
  }

  /**
   * @brief Returns read-only access to the current destination field.
   * @return Current logical destination field.
   */
  [[nodiscard]] const Field& Destination() const noexcept {
    return sourceIsFirst_ ? second_ : first_;
  }

  /**
   * @brief Exchanges the logical source and destination roles in constant time.
   *
   * No field data is copied, and existing references to either physical field remain valid.
   */
  void Swap() noexcept {
    sourceIsFirst_ = !sourceIsFirst_;
  }

  /**
   * @brief Clears both fields and restores the first field as the logical source.
   */
  void Reset() noexcept {
    first_.Clear();
    second_.Clear();

    sourceIsFirst_ = true;
  }

private:
  Field first_;
  Field second_;
  bool sourceIsFirst_ = true;
};
} // namespace fluid_simulation::simulation::cpu
