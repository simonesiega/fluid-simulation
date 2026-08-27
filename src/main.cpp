#include "app/ApplicationConfig.hpp"
#include "simulation/GridCoordinates.hpp"
#include "simulation/SimulationSettings.hpp"
#include "simulation/cpu/SimulationCPU.hpp"

#include <raylib.h>

#include <algorithm>
#include <cmath>

namespace {
using fluid_simulation::config::ApplicationConfig;
using fluid_simulation::simulation::GridCoordinates;
using fluid_simulation::simulation::NormalizedToGrid;
using fluid_simulation::simulation::SimulationSettings;
using fluid_simulation::simulation::cpu::SimulationCPU;

constexpr float resetIndicatorDuration = 0.75F;

// Runtime values shared by the input, update, and render phases.
struct ApplicationState {
  bool isPause = false;
  bool resetRequested = false;
  float frameTime = 0.0F;
  float resetIndicatorTimeRemaining = 0.0F;
  Vector2 mousePosition{};
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
  state.mousePosition = GetMousePosition();
}

/**
 * @brief Advances temporary application state and maps the cursor to the simulation grid.
 * @param state Application state containing timing, viewport, and cursor values.
 * @param simulation CPU simulation that provides the active grid dimensions.
 * @return Nothing.
 */
void Update(ApplicationState& state, const SimulationCPU& simulation) {
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
}

/**
 * @brief Draws the HUD, responsive viewport, and brush preview.
 * @param state Application state to render without modification.
 * @param settings Simulation settings that control the brush preview.
 * @return Nothing.
 */
void Render(const ApplicationState& state, const SimulationSettings& settings) {
  BeginDrawing();
  ClearBackground(BLACK);
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

  if (CheckCollisionPointRec(state.mousePosition, state.viewport)) {
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
    Update(state, simulation);
    Render(state, simulationSettings);
  }

  CloseWindow();
  return 0;
}
