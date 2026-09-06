#include <raylib.h>

#if defined(PLATFORM_WEB)
#include <emscripten/emscripten.h>
#endif

#include "gameMain.h"
#include "gameCanvas.h"
#include "settings.h"
#include "mouse.h"

namespace {
    Settings settings{};
    GameCanvas gameCanvas{};
    Mouse mouse{};

    bool gameInitialized = false;
    bool applicationClosed = false;

    void closeApplication() {
        if (applicationClosed) {
            return;
        }

        if (gameInitialized) {
            closeGame();
            gameInitialized = false;
        }

        gameCanvas.close();
        CloseWindow();
        applicationClosed = true;
    }

    bool updateAndDrawFrame() {
        mouse.update(gameCanvas);

        gameCanvas.begin();
        const bool keepRunning = updateGame(mouse);
        gameCanvas.end();

        BeginDrawing();
        ClearBackground(BLACK);
        gameCanvas.draw();
        EndDrawing();

        return keepRunning;
    }

#if defined(PLATFORM_WEB)
    void updateWebFrame() {
        if (!updateAndDrawFrame()) {
            emscripten_cancel_main_loop();
            closeApplication();
        }
    }
#endif
}

int main() {
    settings.setupGlobal();
    gameCanvas.setUp();

    if (!initGame()) {
        closeApplication();
        return 1;
    }

    gameInitialized = true;

#if defined(PLATFORM_WEB)
    // A value of 0 synchronizes the game with requestAnimationFrame().
    // The final argument prevents main() from continuing while the browser
    // owns the application loop.
    emscripten_set_main_loop(updateWebFrame, 0, 1);
#else
    while (!WindowShouldClose()) {
        if (!updateAndDrawFrame()) {
            break;
        }
    }

    closeApplication();
#endif

    return 0;
}
