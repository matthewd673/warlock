#include <stdlib.h>
#include <stdio.h>

#include "raylib.h"
#include "ll.h"

#define WINDOW_WIDTH    640
#define WINDOW_HEIGHT   480

void update();
void render();

int wallMode = 1;
int mx = 0;
int my = 0;

int main() {
    // create window
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "golem");
    SetTargetFPS(60);
    HideCursor();

    // game loop
    while (!WindowShouldClose()) {
        update();
        render();
    }

    // free camera
    CloseWindow();
}

void update() {
    // input
    mx = GetMouseX();
    my = GetMouseY();

    // switch wall mode
    if (IsKeyPressed(KEY_ONE)) {
        wallMode = 1;
    }
    else if (IsKeyPressed(KEY_TWO)) {
        wallMode = 0;
    }
}

void drawUI() {
    DrawText("Wall mode (1)", 2, 2, 8, wallMode ? GREEN : WHITE);
    DrawText("Sprite mode (2)", 2, 10, 8, wallMode ? WHITE : GREEN);
}

void drawWallEditor() {

}

void drawSpriteEditor() {

}

void render() {
    BeginDrawing();
    ClearBackground(BLACK);

    if (wallMode) {
        drawWallEditor();
    }
    else {
        drawSpriteEditor();
    }

    drawUI();

    // draw cursor
    DrawLine(mx - 3, my, mx + 3, my, WHITE);
    DrawLine(mx, my - 3, mx, my + 3, WHITE);

    EndDrawing();
}