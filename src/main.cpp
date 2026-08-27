#include "app/ApplicationConfig.hpp"
#include "simulation/GridCoordinates.hpp"
#include "simulation/SimulationSettings.hpp"
#include "simulation/cpu/SimulationCPU.hpp"

#include <raylib.h>

#include <algorithm>
#include <cmath>
#include <cstddef>

namespace {
using fluid_simulation::config::ApplicationConfig;
using fluid_simulation::simulation::GridCoordinates;
using fluid_simulation::simulation::NormalizedToGrid;
using fluid_simulation::simulation::SimulationSettings;
using fluid_simulation::simulation::cpu::ScalarField;
using fluid_simulation::simulation::cpu::SimulationCPU;
using fluid_simulation::simulation::cpu::Vector2f;
using fluid_simulation::simulation::cpu::VectorField;

constexpr float resetIndicatorDuration = 0.75F;
constexpr std::size_t velocityDebugStride = 4;
constexpr float velocityDebugScale = 1.0F;
constexpr float velocityDebugMinimumMagnitudeSquared = 0.0001F;

// Runtime values shared by the input, update, and render phases.
struct ApplicationState {
  bool isPause = false;
  bool resetRequested = false;
  bool isLeftMouseButtonDown = false;
  bool mouseInsideViewport = false;
  float frameTime = 0.0F;
  float resetIndicatorTimeRemaining = 0.0F;
  Vector2 mousePosition{};
  Vector2 mouseDelta{};
  Rectangle viewport{};
  Vector2 normalizedMousePosition{};
  GridCoordinates mouseGridCoordinates{};
};

/**
 * @brief Captures keyboard and mouse input for the current frame.
 * @param state Application state updated with input events and the mouse position.
 * @return Nothing.
 */
void HandleInput(ApplicationState& state) {
  if (IsKeyPressed(KEY_SPACE)) {
    state.isPause = !state.isPause;
  }

  state.resetRequested = IsKeyPressed(KEY_R);
  state.isLeftMouseButtonDown = IsMouseButtonDown(MOUSE_BUTTON_LEFT);
  state.mousePosition = GetMousePosition();
  state.mouseDelta = GetMouseDelta();
}

/**
 * @brief Adds density and mouse-drag momentum with shared radial falloff.
 * @param simulation CPU simulation containing the fields to modify.
 * @param settings Simulation settings that define brush radius, density, and force.
 * @param center Grid cell at the center of the radial brush.
 * @param normalizedDeltaX Horizontal mouse movement in normalized simulation space.
 * @param normalizedDeltaY Vertical mouse movement in normalized simulation space.
 * @return Nothing.
 */
void InjectDensityAndVelocity(SimulationCPU& simulation,
                              const SimulationSettings& settings,
                              const GridCoordinates& center,
                              float normalizedDeltaX,
                              float normalizedDeltaY) {
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

  // Keep the edge-safe square bounds as the candidate region for both radial injections.
  const std::size_t width = simulation.Width();
  const std::size_t height = simulation.Height();
  const std::size_t radiusX = static_cast<std::size_t>(normalizedRadius * static_cast<float>(width));
  const std::size_t radiusY = static_cast<std::size_t>(normalizedRadius * static_cast<float>(height));
  const std::size_t minimumX = center.x - std::min(center.x, radiusX);
  const std::size_t maximumX = center.x + std::min(width - 1 - center.x, radiusX);
  const std::size_t minimumY = center.y - std::min(center.y, radiusY);
  const std::size_t maximumY = center.y + std::min(height - 1 - center.y, radiusY);
  ScalarField& density = simulation.Density();
  VectorField& velocityField = simulation.Velocity();

  for (std::size_t y = minimumY; y <= maximumY; ++y) {
    for (std::size_t x = minimumX; x <= maximumX; ++x) {
      // Cast before subtraction so offsets to the left and above remain negative.
      const float offsetX = static_cast<float>(x) - static_cast<float>(center.x);
      const float offsetY = static_cast<float>(y) - static_cast<float>(center.y);
      const float normalizedX = offsetX / static_cast<float>(width);
      const float normalizedY = offsetY / static_cast<float>(height);
      const float distance = std::sqrt(normalizedX * normalizedX + normalizedY * normalizedY);

      if (distance > normalizedRadius) {
        continue;
      }

      const float normalizedDistance = distance / normalizedRadius;
      const float falloff = std::clamp(1.0F - normalizedDistance, 0.0F, 1.0F);

      if (injectDensity) {
        float& value = density.At(x, y);
        value += settings.brushDensity * falloff;
      }

      if (injectVelocity) {
        Vector2f& velocity = velocityField.At(x, y);
        velocity.x += forceX * falloff;
        velocity.y += forceY * falloff;
      }
    }
  }
}

/**
 * @brief Advances application state, maps the cursor, and handles density and velocity injection.
 * @param state Application state containing timing, viewport, and cursor values.
 * @param settings Simulation settings that control brush injection.
 * @param simulation CPU simulation to update.
 * @return Nothing.
 */
void Update(ApplicationState& state, const SimulationSettings& settings, SimulationCPU& simulation) {
  if (state.resetRequested) {
    state.resetIndicatorTimeRemaining = resetIndicatorDuration;
  } else {
    state.resetIndicatorTimeRemaining = std::max(0.0F, state.resetIndicatorTimeRemaining - state.frameTime);
  }

  const float screenWidth = static_cast<float>(GetScreenWidth());
  const float screenHeight = static_cast<float>(GetScreenHeight());
  const float availableViewportWidth = std::max(0.0F, screenWidth - 2.0F * ApplicationConfig::viewportMargin);
  const float viewportTop = ApplicationConfig::hudHeight + ApplicationConfig::viewportMargin;
  const float availableViewportHeight = std::max(0.0F, screenHeight - viewportTop - ApplicationConfig::viewportMargin);
  const float viewportSize = std::min(availableViewportWidth, availableViewportHeight);

  state.viewport = {
    (screenWidth - viewportSize) / 2.0F,
    viewportTop + (availableViewportHeight - viewportSize) / 2.0F,
    viewportSize,
    viewportSize,
  };

  // Keep the normalized position unclamped for accurate cursor reporting outside the viewport.
  state.normalizedMousePosition = {
    (state.mousePosition.x - state.viewport.x) / state.viewport.width,
    (state.mousePosition.y - state.viewport.y) / state.viewport.height,
  };

  state.mouseGridCoordinates =
    NormalizedToGrid(state.normalizedMousePosition.x, state.normalizedMousePosition.y, simulation.Width(), simulation.Height());

  // Keep interaction eligibility separate because grid mapping clamps out-of-viewport positions.
  state.mouseInsideViewport = CheckCollisionPointRec(state.mousePosition, state.viewport);

  if (!state.isPause && state.mouseInsideViewport && state.isLeftMouseButtonDown) {
    // Normalize pixel motion so force remains consistent when the viewport is resized.
    const float normalizedDeltaX = state.mouseDelta.x / state.viewport.width;
    const float normalizedDeltaY = state.mouseDelta.y / state.viewport.height;
    InjectDensityAndVelocity(simulation, settings, state.mouseGridCoordinates, normalizedDeltaX, normalizedDeltaY);
  }
}

/**
 * @brief Draws a scalar field as grayscale cells inside the simulation viewport.
 * @param field Scalar field to visualize without modification.
 * @param viewport Screen-space rectangle occupied by the field.
 * @return Nothing.
 */
void RenderScalarField(const ScalarField& field, const Rectangle& viewport) {
  const std::size_t width = field.Width();
  const std::size_t height = field.Height();

  const float cellWidth = viewport.width / static_cast<float>(width);
  const float cellHeight = viewport.height / static_cast<float>(height);

  for (std::size_t y = 0; y < height; ++y) {
    for (std::size_t x = 0; x < width; ++x) {
      const float value = field.At(x, y);
      const float displayValue = std::isfinite(value) ? std::clamp(value, 0.0F, 1.0F) : 0.0F;
      if (displayValue <= 0.0F) {
        continue;
      }

      const unsigned char intensity = static_cast<unsigned char>(displayValue * 255.0F);
      const Color color = {intensity, intensity, intensity, 255};
      const Rectangle cell = {
        viewport.x + static_cast<float>(x) * cellWidth,
        viewport.y + static_cast<float>(y) * cellHeight,
        cellWidth,
        cellHeight,
      };

      DrawRectangleRec(cell, color);
    }
  }
}

/**
 * @brief Draws sampled velocity vectors from their grid-cell centers.
 * @param field Velocity field to visualize without modification.
 * @param viewport Screen-space rectangle occupied by the field.
 * @return Nothing.
 */
void RenderVelocityField(const VectorField& field, const Rectangle& viewport) {
  const std::size_t width = field.Width();
  const std::size_t height = field.Height();
  const float cellWidth = viewport.width / static_cast<float>(width);
  const float cellHeight = viewport.height / static_cast<float>(height);

  BeginScissorMode(static_cast<int>(viewport.x),
                   static_cast<int>(viewport.y),
                   static_cast<int>(viewport.width),
                   static_cast<int>(viewport.height));

  for (std::size_t y = 0; y < height; y += velocityDebugStride) {
    for (std::size_t x = 0; x < width; x += velocityDebugStride) {
      const Vector2f& velocity = field.At(x, y);
      if (!std::isfinite(velocity.x) || !std::isfinite(velocity.y)) {
        continue;
      }

      const float magnitudeSquared = velocity.x * velocity.x + velocity.y * velocity.y;
      if (magnitudeSquared <= velocityDebugMinimumMagnitudeSquared) {
        continue;
      }

      const Vector2 start = {
        viewport.x + (static_cast<float>(x) + 0.5F) * cellWidth,
        viewport.y + (static_cast<float>(y) + 0.5F) * cellHeight,
      };
      const Vector2 end = {
        start.x + velocity.x * velocityDebugScale,
        start.y + velocity.y * velocityDebugScale,
      };

      if (std::isfinite(end.x) && std::isfinite(end.y)) {
        DrawLineV(start, end, RED);
      }
    }
  }

  EndScissorMode();
}

/**
 * @brief Draws debug fields, the HUD, responsive viewport, and brush preview.
 * @param state Application state to render without modification.
 * @param settings Simulation settings that control the brush preview.
 * @param simulation CPU simulation containing the fields to visualize.
 * @return Nothing.
 */
void Render(const ApplicationState& state, const SimulationSettings& settings, const SimulationCPU& simulation) {
  BeginDrawing();
  ClearBackground(BLACK);
  RenderScalarField(simulation.Density(), state.viewport);
  RenderVelocityField(simulation.Velocity(), state.viewport);

  DrawText(ApplicationConfig::windowTitle, 16, 16, 20, LIGHTGRAY);

  const char* stateText = state.isPause ? "Paused" : "Running";
  const int stateTextWidth = MeasureText(stateText, 20);
  DrawText(stateText, (GetScreenWidth() - stateTextWidth) / 2, 16, 20, LIGHTGRAY);

  if (state.resetIndicatorTimeRemaining > 0.0F) {
    const char* resetText = "Reset";
    const int resetTextWidth = MeasureText(resetText, 20);
    DrawText(resetText, (GetScreenWidth() - resetTextWidth) / 2, 40, 20, LIGHTGRAY);
  }

  const char* fpsText = TextFormat("%d FPS", GetFPS());
  const int fpsTextWidth = MeasureText(fpsText, 20);
  DrawText(fpsText, GetScreenWidth() - fpsTextWidth - 16, 16, 20, LIGHTGRAY);

  const float frameTimeMilliseconds = std::isfinite(state.frameTime) ? state.frameTime * 1000.0F : 0.0F;
  const char* frameTimeText = TextFormat("%.2f ms", frameTimeMilliseconds);
  const int frameTimeTextWidth = MeasureText(frameTimeText, 20);
  DrawText(frameTimeText, GetScreenWidth() - frameTimeTextWidth - 16, 40, 20, LIGHTGRAY);

  const char* mousePositionText = TextFormat("Mouse: (%.3f, %.3f)", state.normalizedMousePosition.x, state.normalizedMousePosition.y);
  DrawText(mousePositionText, 16, 40, 20, LIGHTGRAY);

  if (state.mouseInsideViewport) {
    // Prevent the brush outline from drawing across the viewport border.
    BeginScissorMode(static_cast<int>(state.viewport.x),
                     static_cast<int>(state.viewport.y),
                     static_cast<int>(state.viewport.width),
                     static_cast<int>(state.viewport.height));
    const float brushPreviewRadius = settings.brushRadius * state.viewport.width;
    DrawCircleLinesV(state.mousePosition, brushPreviewRadius, LIGHTGRAY);
    EndScissorMode();
  }

  DrawRectangleLinesEx(state.viewport, ApplicationConfig::viewportBorderWidth, LIGHTGRAY);
  EndDrawing();
}
} // namespace

/**
 * @brief Initializes the window and runs the application frame loop.
 * @return Zero when the application exits normally.
 */
int main() {
  SetConfigFlags(FLAG_WINDOW_RESIZABLE);
  InitWindow(ApplicationConfig::windowWidth, ApplicationConfig::windowHeight, ApplicationConfig::windowTitle);
  SetWindowMinSize(ApplicationConfig::minimumWindowWidth, ApplicationConfig::minimumWindowHeight);
  SetTargetFPS(ApplicationConfig::defaultTargetFps);

  Image windowIcon = LoadImage("assets/icons/logo-32-black.png");
  if (IsImageValid(windowIcon)) {
    SetWindowIcon(windowIcon);
    UnloadImage(windowIcon);
  }

  ApplicationState state;
  SimulationSettings simulationSettings;
  SimulationCPU simulation(simulationSettings);

  while (!WindowShouldClose()) {
    // Record timing before running the frame's input, update, and render phases.
    state.frameTime = GetFrameTime();

    HandleInput(state);
    Update(state, simulationSettings, simulation);
    Render(state, simulationSettings, simulation);
  }

  CloseWindow();
  return 0;
}
