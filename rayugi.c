#include "raylib.h"
#include "raygui.h"

int main(void) {
    // Initialization
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib [raygui] example - basic window");

    char button_text[] = "Click me!";
    bool buttonClicked = false;

    SetTargetFPS(60); // Set our game to run at 60 frames-per-second

    // Main game loop
    while (!WindowShouldClose()) { // Detect window close button or ESC key
        // Update
        if (GuiButton((Rectangle) { screenWidth / 2 - 50, screenHeight / 2 - 25, 100, 50 }, button_text)) {
            buttonClicked = !buttonClicked;
            if (buttonClicked) {
                strcpy(button_text, "Clicked!");
            }
            else {
                strcpy(button_text, "Click me!");
            }
        }

        // Draw
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawText("Welcome to the raygui example!", 190, 200, 20, LIGHTGRAY);
        EndDrawing();
    }

    // De-Initialization
    CloseWindow(); // Close window and OpenGL context

    return 0;
}
