#include <raylib.h>

int main()
{
    constexpr int windowWidth = 1280;
    constexpr int windowHeight = 720;
    constexpr int targetFps = 60;
    constexpr const char* windowTitle = "Fluid Simulation";

    InitWindow(windowWidth, windowHeight, windowTitle);
    SetTargetFPS(targetFps);

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
        DrawText(windowTitle, 16, 16, 20, LIGHTGRAY);

        const char* fpsText = TextFormat("%d FPS", GetFPS());
        const int fpsTextWidth = MeasureText(fpsText, 20);
        DrawText(fpsText, GetScreenWidth() - fpsTextWidth - 16, 16, 20, LIGHTGRAY);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
