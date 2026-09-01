#pragma once

namespace fluid_simulation::config {
/**
 * @brief Defines immutable application-window and viewport defaults.
 *
 * Window dimensions and layout distances are expressed in screen pixels.
 */
struct ApplicationConfig final {
  static constexpr int windowWidth = 1280;
  static constexpr int windowHeight = 720;
  static constexpr int minimumWindowWidth = 640;
  static constexpr int minimumWindowHeight = 480;
  static constexpr const char* windowTitle = "Fluid Simulation";
  static constexpr int defaultTargetFps = 120;

  static constexpr float hudHeight = 60.0F;
  static constexpr float viewportMargin = 16.0F;
  static constexpr float viewportBorderWidth = 2.0F;
};
} // namespace fluid_simulation::config
