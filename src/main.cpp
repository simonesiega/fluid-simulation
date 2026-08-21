#include <raylib.h>

int main()
{
    constexpr int windowWidth = 1280;
    constexpr int windowHeight = 720;

    InitWindow(windowWidth, windowHeight, "Fluid Simulation");

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
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
