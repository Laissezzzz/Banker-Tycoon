#pragma once

#include <raylib.h>
#include <string>

class Settings {
private:
    inline static constexpr int screenWidth = 800;
    inline static constexpr int screenHeight = 450;

    inline static constexpr int virtualWidth = 800;
    inline static constexpr int virtualHeight = 450;

    inline static constexpr int targetFPS = 60;
    inline static constexpr bool windowResizable = true;
    inline static constexpr int closeKey = 0;

    inline static const std::string title = "Banker Tycoon";
    inline static constexpr Color clearColor = BLACK;

public:
    static void setupGlobal() {
#if PRODUCTION_BUILD
        SetTraceLogLevel(LOG_NONE);
#endif

#if !defined(PLATFORM_WEB)
        if constexpr (windowResizable) {
            SetConfigFlags(FLAG_WINDOW_RESIZABLE);
        }
#endif

        InitWindow(screenWidth, screenHeight, title.c_str());
        SetExitKey(closeKey);
        SetTargetFPS(targetFPS);
    }

    static constexpr Color getClearColor() {
        return clearColor;
    }

    static constexpr int getWidth() {
        return screenWidth;
    }

    static constexpr int getHeight() {
        return screenHeight;
    }

    static constexpr int getVirtualWidth() {
        return virtualWidth;
    }

    static constexpr int getVirtualHeight() {
        return virtualHeight;
    }
};
