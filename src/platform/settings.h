#pragma once

#include <raylib.h>
#include <string>

class Settings {
private:
    // General settings
    inline static constexpr bool productionBuild = false;

    // Window
    inline static int screenWidth = 800;
    inline static int screenHeight = screenWidth;
    inline static std::string title = "Banker Tycoon";
    inline static bool windowResizable = true;
    inline static bool closeOnEscape = false;

    // Canvas
    inline static constexpr int virtualWidth = 800;
    inline static constexpr int virtualHeight = 450;

    // Rendering
    inline static int targetFPS = 120;
    inline static Color clearColor = BLACK;

    // Audio
    inline static float masterVolume = 1.0f;

public:
    static void setupGlobal() {
        if constexpr (productionBuild) {
            SetTraceLogLevel(LOG_NONE);
        }

        if (windowResizable) {
            SetConfigFlags(FLAG_WINDOW_RESIZABLE);
        }

        InitWindow(screenWidth,screenHeight,title.c_str());

        SetExitKey(closeOnEscape);

        SetTargetFPS(targetFPS);
    }

    //Getter functions
    Color getClearColor() const { return clearColor; }
    int getWidth() const { return screenWidth; }
    int getHeight() const { return screenHeight; }
    static int getVirtualWidth() {return virtualWidth;}
    static int getVirtualHeight() {return virtualHeight;}
};