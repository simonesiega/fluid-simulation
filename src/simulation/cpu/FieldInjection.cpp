#include "simulation/cpu/FieldInjection.hpp"

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <stdexcept>

namespace fluid_simulation::simulation::cpu {
void InjectDensityAndVelocity(SimulationCPU& simulation,
                              const SimulationSettings& settings,
                              const GridCoordinates& center,
                              float normalizedDeltaX,
                              float normalizedDeltaY) {
  const std::size_t width = simulation.Width();
  const std::size_t height = simulation.Height();

  if (center.x >= width || center.y >= height) {
    throw std::out_of_range("Brush center is outside the simulation grid");
  }

  const bool injectDensity = std::isfinite(settings.brushDensity) && settings.brushDensity > 0.0F;
  const bool validForce =
    std::isfinite(settings.brushForce) && settings.brushForce > 0.0F && std::isfinite(normalizedDeltaX) && std::isfinite(normalizedDeltaY);

  const float forceX = validForce ? normalizedDeltaX * settings.brushForce : 0.0F;
  const float forceY = validForce ? normalizedDeltaY * settings.brushForce : 0.0F;
  const bool injectVelocity = validForce && std::isfinite(forceX) && std::isfinite(forceY) && (forceX != 0.0F || forceY != 0.0F);

  if (!injectDensity && !injectVelocity) {
    return;
  }

  const float normalizedRadius = std::isfinite(settings.brushRadius) ? std::clamp(settings.brushRadius, 0.0F, 1.0F) : 0.0F;
  if (normalizedRadius <= 0.0F) {
    return;
  }

  // Scale each axis independently to keep the brush circular on non-square grids.
  const float inverseWidth = 1.0F / static_cast<float>(width);
  const float inverseHeight = 1.0F / static_cast<float>(height);
  const float radiusSquared = normalizedRadius * normalizedRadius;

  const std::size_t radiusX = static_cast<std::size_t>(normalizedRadius * static_cast<float>(width));
  const std::size_t radiusY = static_cast<std::size_t>(normalizedRadius * static_cast<float>(height));

  const std::size_t minimumX = center.x - std::min(center.x, radiusX);
  const std::size_t maximumX = center.x + std::min(width - 1 - center.x, radiusX);
  const std::size_t minimumY = center.y - std::min(center.y, radiusY);
  const std::size_t maximumY = center.y + std::min(height - 1 - center.y, radiusY);

  const float centerX = static_cast<float>(center.x);
  const float centerY = static_cast<float>(center.y);

  ScalarField& density = simulation.DensitySource();
  VectorField& velocityField = simulation.VelocitySource();

  for (std::size_t y = minimumY; y <= maximumY; ++y) {
    for (std::size_t x = minimumX; x <= maximumX; ++x) {
      const float normalizedX = (static_cast<float>(x) - centerX) * inverseWidth;
      const float normalizedY = (static_cast<float>(y) - centerY) * inverseHeight;
      const float distanceSquared = normalizedX * normalizedX + normalizedY * normalizedY;

      // Reject outside cells before the square root required by linear falloff.
      if (distanceSquared > radiusSquared) {
        continue;
      }

      const float normalizedDistance = std::sqrt(distanceSquared) / normalizedRadius;
      const float falloff = std::clamp(1.0F - normalizedDistance, 0.0F, 1.0F);

      if (injectDensity) {
        density.At(x, y) += settings.brushDensity * falloff;
      }

      if (injectVelocity) {
        Vector2f& velocity = velocityField.At(x, y);
        velocity.x += forceX * falloff;
        velocity.y += forceY * falloff;
      }
    }
  }
}
} // namespace fluid_simulation::simulation::cpu
