#pragma once

#include <raylib.h>

#include "gameCanvas.h"

class Mouse {
private:
    Vector2 position{};
    Vector2 lastTouchScreenPosition{};

    bool insideCanvas = false;
    bool leftPressed = false;
    bool leftDown = false;
    bool leftReleased = false;
    bool touchWasDown = false;
    bool touchInput = false;

public:
    void update(const GameCanvas& gameCanvas) {
        const bool touchDown = GetTouchPointCount() > 0;
        Vector2 screenPosition{};

        if (touchDown) {
            touchInput = true;
            lastTouchScreenPosition = GetTouchPosition(0);
            screenPosition = lastTouchScreenPosition;

            leftPressed = !touchWasDown;
            leftDown = true;
            leftReleased = false;
        }
        else if (touchWasDown) {
            // On release Raylib no longer provides the touch position,
            // so the final event uses the last valid position.
            touchInput = true;
            screenPosition = lastTouchScreenPosition;

            leftPressed = false;
            leftDown = false;
            leftReleased = true;
        }
        else {
            touchInput = false;
            screenPosition = ::GetMousePosition();

            leftPressed = IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
            leftDown = IsMouseButtonDown(MOUSE_BUTTON_LEFT);
            leftReleased = IsMouseButtonReleased(MOUSE_BUTTON_LEFT);
        }

        position = gameCanvas.getCanvasPosition(screenPosition);
        insideCanvas = gameCanvas.isScreenPositionInside(screenPosition);
        touchWasDown = touchDown;
    }

    Vector2 getPosition() const {
        return position;
    }

    bool isInsideCanvas() const {
        return insideCanvas;
    }

    bool isLeftPressed() const {
        return leftPressed;
    }

    bool isLeftDown() const {
        return leftDown;
    }

    bool isLeftReleased() const {
        return leftReleased;
    }

    bool isTouchInput() const {
        return touchInput;
    }

    bool isInside(Rectangle rectangle) const {
        return insideCanvas && CheckCollisionPointRec(position, rectangle);
    }

    void drawCoordinates() const {
        const char* coordinateText = TextFormat(
            "X: %i  Y: %i",
            static_cast<int>(position.x),
            static_cast<int>(position.y)
        );

        constexpr int fontSize = 16;
        constexpr float padding = 5.0f;
        const int textWidth = MeasureText(coordinateText, fontSize);

        const Rectangle background{
            position.x + 12.0f,
            position.y + 12.0f,
            static_cast<float>(textWidth) + padding * 2.0f,
            static_cast<float>(fontSize) + padding * 2.0f
        };

        DrawCircleV(position, 3.0f, RED);
        DrawRectangleRec(background, Fade(BLACK, 0.8f));
        DrawText(
            coordinateText,
            static_cast<int>(background.x + padding),
            static_cast<int>(background.y + padding),
            fontSize,
            WHITE
        );
    }

    void drawCrosshair() const {
        constexpr float crosshairSize = 6.0f;

        DrawLine(
            static_cast<int>(position.x - crosshairSize),
            static_cast<int>(position.y),
            static_cast<int>(position.x + crosshairSize),
            static_cast<int>(position.y),
            RED
        );

        DrawLine(
            static_cast<int>(position.x),
            static_cast<int>(position.y - crosshairSize),
            static_cast<int>(position.x),
            static_cast<int>(position.y + crosshairSize),
            RED
        );
    }

    void drawDebug() const {
        if (!insideCanvas) {
            return;
        }

        drawCrosshair();
        drawCoordinates();
    }
};
