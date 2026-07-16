#include <raylib.h>
#include <gameMain.h>
#include "gameCanvas.h"
#include "settings.h"
#include "imGuiHandler.h"

int main() {
    Settings settings;
    GameCanvas gameCanvas;

    settings.setupGlobal();
    gameCanvas.setUp();
    ImGuiHandler::initialize(settings);

    if (!initGame()) {
        ImGuiHandler::shutdown();
        CloseWindow();
        return 1;
    }

    bool running = true;

    while (!WindowShouldClose() && running) {

        gameCanvas.begin();
        if (!updateGame()) {
            running = false;
        }
        gameCanvas.end();

        BeginDrawing();
        ClearBackground(BLACK);

        gameCanvas.draw();

        ImGuiHandler::beginFrame();

        // Interfaccia ImGui

        ImGuiHandler::endFrame();

        EndDrawing();
    }

    closeGame();
    ImGuiHandler::shutdown();
    CloseWindow();

    return 0;
}