#include "app/ApplicationConfig.hpp"

#include <raylib.h>

#include <algorithm>
#include <cmath>

int main() {
  using fluid_simulation::config::ApplicationConfig;

  SetConfigFlags(FLAG_WINDOW_RESIZABLE);
  InitWindow(ApplicationConfig::windowWidth, ApplicationConfig::windowHeight, ApplicationConfig::windowTitle);
  SetWindowMinSize(ApplicationConfig::minimumWindowWidth, ApplicationConfig::minimumWindowHeight);
  SetTargetFPS(ApplicationConfig::defaultTargetFps);

  Image windowIcon = LoadImage("assets/icons/logo-32-black.png");
  if (IsImageValid(windowIcon)) {
    SetWindowIcon(windowIcon);
    UnloadImage(windowIcon);
  }

  while (!WindowShouldClose()) {
    const float frameTime = GetFrameTime();
    const float frameTimeMilliseconds = std::isfinite(frameTime) ? frameTime * 1000.0F : 0.0F;
    const Vector2 mousePosition = GetMousePosition();

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

    BeginDrawing();
    ClearBackground(BLACK);
    DrawText(ApplicationConfig::windowTitle, 16, 16, 20, LIGHTGRAY);

    const char* fpsText = TextFormat("%d FPS", GetFPS());
    const int fpsTextWidth = MeasureText(fpsText, 20);
    DrawText(fpsText, GetScreenWidth() - fpsTextWidth - 16, 16, 20, LIGHTGRAY);

    const char* frameTimeText = TextFormat("%.2f ms", frameTimeMilliseconds);
    const int frameTimeTextWidth = MeasureText(frameTimeText, 20);
    DrawText(frameTimeText, GetScreenWidth() - frameTimeTextWidth - 16, 40, 20, LIGHTGRAY);

    const char* mousePositionText = TextFormat("Mouse: (%d, %d)", static_cast<int>(mousePosition.x), static_cast<int>(mousePosition.y));
    DrawText(mousePositionText, 16, 40, 20, LIGHTGRAY);

    DrawRectangleLinesEx(viewport, ApplicationConfig::viewportBorderWidth, LIGHTGRAY);
    EndDrawing();
  }

  CloseWindow();
  return 0;
}
