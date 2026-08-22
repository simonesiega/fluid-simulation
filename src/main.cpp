#include "app/ApplicationConfig.hpp"

#include <raylib.h>

int main()
{
    using fluid_simulation::config::ApplicationConfig;

    InitWindow(ApplicationConfig::windowWidth,
               ApplicationConfig::windowHeight,
               ApplicationConfig::windowTitle);
    SetTargetFPS(ApplicationConfig::defaultTargetFps);

    Image windowIcon = LoadImage("assets/icons/logo-32.png");
    if (IsImageValid(windowIcon))
    {
        SetWindowIcon(windowIcon);
        UnloadImage(windowIcon);
    }

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(BLACK);
        DrawText(ApplicationConfig::windowTitle, 16, 16, 20, LIGHTGRAY);

        const char* fpsText = TextFormat("%d FPS", GetFPS());
        const int fpsTextWidth = MeasureText(fpsText, 20);
        DrawText(fpsText, GetScreenWidth() - fpsTextWidth - 16, 16, 20, LIGHTGRAY);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
