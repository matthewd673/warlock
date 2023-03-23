#include <stdlib.h>
#include <stdio.h>

#include "raylib.h"
#include "ll.h"
#include "editor.h"

#define WINDOW_WIDTH    640
#define WINDOW_HEIGHT   480

void update();
void render();


Workspace work;
int wallMode = 1;
int mx = 0;
int my = 0;

int wallDown = 0;
int wallDownX = 0;
int wallDownY = 0;

int main() {
    // create window
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "golem");
    SetTargetFPS(60);
    SetExitKey(KEY_NULL);
    HideCursor();

    // init
    work = new_Workspace();

    // game loop
    while (!WindowShouldClose()) {
        update();
        render();
    }

    // free camera
    CloseWindow();
}

void changeMode(int mode) {
    wallMode = mode;
    wallDown = 0;
}

void update() {
    // input
    mx = GetMouseX();
    my = GetMouseY();

    // switch wall mode
    if (IsKeyPressed(KEY_ONE)) {
        changeMode(1);
    }
    else if (IsKeyPressed(KEY_TWO)) {
        changeMode(0);
    }

    // break wall chain
    if (IsKeyPressed(KEY_ESCAPE)) {
        wallDown = 0;
    }

    // handle mouse input
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        // sprite mode
        if (!wallMode) {
            LL_add(Workspace_getSprites(work), new_Sprite(mx, my));
        }
        // wall mode
        else {
            // place first part of wall
            if (!wallDown) {
                wallDown = 1;
                wallDownX = mx;
                wallDownY = my;
            }
            // place second part of wall and formalize
            else {
                LL_add(Workspace_getWalls(work),
                    new_Wall(wallDownX, wallDownY, mx, my));

                wallDownX = mx;
                wallDownY = my;
            }
        }
    }
}

void drawUI() {
    DrawText("Wall mode (1)", 2, 2, 8, wallMode ? GREEN : WHITE);
    DrawText("Sprite mode (2)", 2, 10, 8, wallMode ? WHITE : GREEN);
}

void drawWalls() {
    // iterate through every wall
    LL walls = Workspace_getWalls(work);
    LNode n = LL_getHead(walls);
    while (n != NULL) {
        // draw wall
        Wall w = (Wall)LNode_getContent(n);
        DrawLine(
            Wall_getX1(w), Wall_getY1(w),
            Wall_getX2(w), Wall_getY2(w),
            WHITE
        );
        n = LNode_getNext(n);
    }

    // draw wall hint
    if (wallDown) {
        DrawLine(
            wallDownX, wallDownY,
            mx, my,
            GRAY
        );
    }
}

void drawSprites() {
    // iterate through every sprite
    LL sprites = Workspace_getSprites(work);
    LNode n = LL_getHead(sprites);
    while (n != NULL) {
        // draw sprite
        Sprite s = (Sprite)LNode_getContent(n);
        DrawCircle(Sprite_getX(s), Sprite_getY(s), 2, WHITE);

        n = LNode_getNext(n);
    }
}

void render() {
    BeginDrawing();
    ClearBackground(BLACK);

    drawWalls();
    drawSprites();

    drawUI();

    // draw cursor
    DrawLine(mx - 3, my, mx + 3, my, WHITE);
    DrawLine(mx, my - 3, mx, my + 3, WHITE);

    EndDrawing();
}