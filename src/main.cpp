#include "app/ApplicationConfig.hpp"

#include <raylib.h>

#include <algorithm>
#include <cmath>

namespace {
using fluid_simulation::config::ApplicationConfig;

constexpr float brushPreviewRadius = 24.0F;
constexpr float resetIndicatorDuration = 0.75F;

void HandleInput(bool& isPause, bool& resetRequested, Vector2& mousePosition) {
  if (IsKeyPressed(KEY_SPACE)) {
    isPause = !isPause;
  }

  resetRequested = IsKeyPressed(KEY_R);
  mousePosition = GetMousePosition();
}

void Update(float frameTime, bool resetRequested, float& resetIndicatorTimeRemaining) {
  if (resetRequested) {
    resetIndicatorTimeRemaining = resetIndicatorDuration;
  } else {
    resetIndicatorTimeRemaining = std::max(0.0F, resetIndicatorTimeRemaining - frameTime);
  }
}

void Render(float frameTimeMilliseconds, bool isPause, float resetIndicatorTimeRemaining, const Vector2& mousePosition) {
  const float screenWidth = static_cast<float>(GetScreenWidth());
  const float screenHeight = static_cast<float>(GetScreenHeight());
  const float availableViewportWidth = std::max(0.0F, screenWidth - 2.0F * ApplicationConfig::viewportMargin);
  const float viewportTop = ApplicationConfig::hudHeight + ApplicationConfig::viewportMargin;
  const float availableViewportHeight = std::max(0.0F, screenHeight - viewportTop - ApplicationConfig::viewportMargin);
  const float viewportSize = std::min(availableViewportWidth, availableViewportHeight);

  const Rectangle viewport = {
    (screenWidth - viewportSize) / 2.0F,
    viewportTop + (availableViewportHeight - viewportSize) / 2.0F,
    viewportSize,
    viewportSize,
  };

  const Vector2 normalizedMousePosition = {
    (mousePosition.x - viewport.x) / viewport.width,
    (mousePosition.y - viewport.y) / viewport.height,
  };

  BeginDrawing();
  ClearBackground(BLACK);
  DrawText(ApplicationConfig::windowTitle, 16, 16, 20, LIGHTGRAY);

  const char* stateText = isPause ? "Paused" : "Running";
  const int stateTextWidth = MeasureText(stateText, 20);
  DrawText(stateText, (GetScreenWidth() - stateTextWidth) / 2, 16, 20, LIGHTGRAY);

  if (resetIndicatorTimeRemaining > 0.0F) {
    const char* resetText = "Reset";
    const int resetTextWidth = MeasureText(resetText, 20);
    DrawText(resetText, (GetScreenWidth() - resetTextWidth) / 2, 40, 20, LIGHTGRAY);
  }

  const char* fpsText = TextFormat("%d FPS", GetFPS());
  const int fpsTextWidth = MeasureText(fpsText, 20);
  DrawText(fpsText, GetScreenWidth() - fpsTextWidth - 16, 16, 20, LIGHTGRAY);

  const char* frameTimeText = TextFormat("%.2f ms", frameTimeMilliseconds);
  const int frameTimeTextWidth = MeasureText(frameTimeText, 20);
  DrawText(frameTimeText, GetScreenWidth() - frameTimeTextWidth - 16, 40, 20, LIGHTGRAY);

  const char* mousePositionText = TextFormat("Mouse: (%.3f, %.3f)", normalizedMousePosition.x, normalizedMousePosition.y);
  DrawText(mousePositionText, 16, 40, 20, LIGHTGRAY);

  if (CheckCollisionPointRec(mousePosition, viewport)) {
    BeginScissorMode(static_cast<int>(viewport.x),
                     static_cast<int>(viewport.y),
                     static_cast<int>(viewport.width),
                     static_cast<int>(viewport.height));
    DrawCircleLinesV(mousePosition, brushPreviewRadius, LIGHTGRAY);
    EndScissorMode();
  }

  DrawRectangleLinesEx(viewport, ApplicationConfig::viewportBorderWidth, LIGHTGRAY);
  EndDrawing();
}
} // namespace

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

  bool isPause = false;
  bool resetRequested = false;
  float resetIndicatorTimeRemaining = 0.0F;
  Vector2 mousePosition{};

  while (!WindowShouldClose()) {
    const float frameTime = GetFrameTime();
    const float frameTimeMilliseconds = std::isfinite(frameTime) ? frameTime * 1000.0F : 0.0F;

    HandleInput(isPause, resetRequested, mousePosition);
    Update(frameTime, resetRequested, resetIndicatorTimeRemaining);
    Render(frameTimeMilliseconds, isPause, resetIndicatorTimeRemaining, mousePosition);
  }

  CloseWindow();
  return 0;
}
