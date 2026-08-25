#include <raylib.h>

#include "gameMain.h"
#include "gameCanvas.h"
#include "settings.h"
#include "imGuiHandler.h"
#include "mouse.h"

int main() {
    Settings settings{};
    GameCanvas gameCanvas{};
    Mouse mouse{};

    settings.setupGlobal();
    gameCanvas.setUp();

    ImGuiHandler::initialize(settings);

    if (!initGame()) {
        ImGuiHandler::shutdown();
        gameCanvas.close();
        CloseWindow();
        return 1;
    }

    bool running = true;

    while (!WindowShouldClose() && running) {
        ImGuiHandler::beginFrame();

        mouse.update(gameCanvas);

        gameCanvas.begin();

        if (!updateGame(mouse)) {
            running = false;
        }

        gameCanvas.end();

        BeginDrawing();
        ClearBackground(BLACK);

        gameCanvas.draw();

        // Interfaccia ImGui
        ImGuiHandler::endFrame();

        EndDrawing();
    }

    closeGame();

    ImGuiHandler::shutdown();
    gameCanvas.close();
    CloseWindow();

    return 0;
}