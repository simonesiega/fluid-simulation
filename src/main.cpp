#include "app/ApplicationConfig.hpp"

#include <raylib.h>

#include <cmath>

int main() {
  using fluid_simulation::config::ApplicationConfig;

  InitWindow(ApplicationConfig::windowWidth,
             ApplicationConfig::windowHeight,
             ApplicationConfig::windowTitle);
  SetTargetFPS(ApplicationConfig::defaultTargetFps);

  Image windowIcon = LoadImage("assets/icons/logo-32.png");
  if (IsImageValid(windowIcon)) {
    SetWindowIcon(windowIcon);
    UnloadImage(windowIcon);
  }

  while (!WindowShouldClose()) {
    const float frameTime = GetFrameTime();
    const float frameTimeMilliseconds = std::isfinite(frameTime) ? frameTime * 1000.0F : 0.0F;

    BeginDrawing();
    ClearBackground(BLACK);
    DrawText(ApplicationConfig::windowTitle, 16, 16, 20, LIGHTGRAY);

    const char* fpsText = TextFormat("%d FPS", GetFPS());
    const int fpsTextWidth = MeasureText(fpsText, 20);
    DrawText(fpsText, GetScreenWidth() - fpsTextWidth - 16, 16, 20, LIGHTGRAY);

    const char* frameTimeText = TextFormat("%.2f ms", frameTimeMilliseconds);
    const int frameTimeTextWidth = MeasureText(frameTimeText, 20);
    DrawText(frameTimeText, GetScreenWidth() - frameTimeTextWidth - 16, 40, 20, LIGHTGRAY);
    EndDrawing();
  }

  CloseWindow();
  return 0;
}
