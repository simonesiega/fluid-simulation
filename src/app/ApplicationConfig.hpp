#pragma once

namespace fluid_simulation::config {
struct ApplicationConfig final {
  static constexpr int windowWidth = 1280;
  static constexpr int windowHeight = 720;
  static constexpr const char* windowTitle = "Fluid Simulation";
  static constexpr int defaultTargetFps = 120;
};
}
