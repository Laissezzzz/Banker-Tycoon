#pragma once

#include <raylib.h>

#include "gameCanvas.h"

class Mouse {
private:
    Vector2 position{};

    bool insideCanvas = false;
    bool leftPressed = false;
    bool leftDown = false;
    bool leftReleased = false;

public:
    void update(const GameCanvas& gameCanvas) {
        position = gameCanvas.getMousePosition();
        insideCanvas = gameCanvas.isMouseInside();

        leftPressed = IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
        leftDown = IsMouseButtonDown(MOUSE_BUTTON_LEFT);
        leftReleased = IsMouseButtonReleased(MOUSE_BUTTON_LEFT);
    }

    Vector2 getPosition() const { return position; }

    bool isInsideCanvas() const { return insideCanvas; }
    bool isLeftPressed() const { return leftPressed; }
    bool isLeftDown() const { return leftDown; }
    bool isLeftReleased() const { return leftReleased; }

    bool isInside(Rectangle rectangle) const {
        return insideCanvas && CheckCollisionPointRec(position, rectangle);
    }

    void drawCoordinates() const {
        Vector2 mousePosition = getPosition();

        const char* coordinateText = TextFormat(
            "X: %i  Y: %i",
            static_cast<int>(mousePosition.x),
            static_cast<int>(mousePosition.y)
        );

        int fontSize = 16;
        int textWidth = MeasureText(coordinateText, fontSize);
        float padding = 5.0f;

        Rectangle background{
            mousePosition.x + 12.0f,
            mousePosition.y + 12.0f,
            static_cast<float>(textWidth) + padding * 2.0f,
            static_cast<float>(fontSize) + padding * 2.0f
        };

        DrawCircleV(mousePosition, 3.0f, RED);
        DrawRectangleRec(background, Fade(BLACK, 0.8f));

        DrawText(coordinateText,static_cast<int>(background.x + padding),static_cast<int>(background.y + padding),fontSize,WHITE);
    }

    void drawCrosshair() const {
        Vector2 mousePosition = getPosition();
        float crosshairSize = 6.0f;

        DrawLine(static_cast<int>(mousePosition.x - crosshairSize),static_cast<int>(mousePosition.y),static_cast<int>(mousePosition.x + crosshairSize),static_cast<int>(mousePosition.y),RED);

        DrawLine(static_cast<int>(mousePosition.x),static_cast<int>(mousePosition.y - crosshairSize), static_cast<int>(mousePosition.x),static_cast<int>(mousePosition.y + crosshairSize),RED);
    }
    
    void drawDebug() const {
        if (!insideCanvas) {
            return;
        }

        drawCrosshair();
        drawCoordinates();
    }
};