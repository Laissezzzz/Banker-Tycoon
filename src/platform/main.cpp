#include <raylib.h>

#include "gameMain.h"
#include "gameCanvas.h"
#include "settings.h"
#include "mouse.h"

int main() {
    Settings settings{};
    GameCanvas gameCanvas{};
    Mouse mouse{};

    settings.setupGlobal();
    gameCanvas.setUp();


    if (!initGame()) {
        gameCanvas.close();
        CloseWindow();
        return 1;
    }

    bool running = true;

    while (!WindowShouldClose() && running) {

        mouse.update(gameCanvas);

        gameCanvas.begin();

        if (!updateGame(mouse)) {
            running = false;
        }

        gameCanvas.end();

        BeginDrawing();
        ClearBackground(BLACK);

        gameCanvas.draw();


        EndDrawing();
    }

    closeGame();

    gameCanvas.close();
    CloseWindow();

    return 0;
}