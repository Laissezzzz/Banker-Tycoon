#pragma once

#include <raylib.h>
#include <string>

#include "mouse.h"

class Button {
private:
    Rectangle rectangle{ 0.0f, 0.0f, 120.0f, 50.0f };
    std::string text = "Button";

    bool pressed = false;
    bool pointerInside = false;

    Color normalColor = LIGHTGRAY;
    Color hoverColor = YELLOW;
    Color pressedColor = GREEN;
    Color borderColor = BLACK;
    Color textColor = BLACK;

    float borderThickness = 2.0f;
    int fontSize = 20;

public:
    // Constructors
    Button() = default;
    Button(const std::string& newText, Rectangle newRectangle) : rectangle(newRectangle), text(newText) {}

    // Getter functions
    const Rectangle& getRectangle() const { return rectangle; }
    const std::string& getText() const { return text; }
    bool isPressed() const { return pressed && pointerInside; }
    bool isPointerInside() const { return pointerInside; }
    Color getNormalColor() const { return normalColor; }
    Color getHoverColor() const { return hoverColor; }
    Color getPressedColor() const { return pressedColor; }
    Color getBorderColor() const { return borderColor; }
    Color getTextColor() const { return textColor; }
    float getBorderThickness() const { return borderThickness; }
    int getFontSize() const { return fontSize; }

    // Setter functions
    void setRectangle(Rectangle newRectangle) { rectangle = newRectangle; }
    void setPosition(Vector2 newPosition) { rectangle.x = newPosition.x; rectangle.y = newPosition.y; }
    void setSize(Vector2 newSize) { rectangle.width = newSize.x; rectangle.height = newSize.y; }
    void setText(const std::string& newText) { text = newText; }
    void setNormalColor(Color newColor) { normalColor = newColor; }
    void setHoverColor(Color newColor) { hoverColor = newColor; }
    void setPressedColor(Color newColor) { pressedColor = newColor; }
    void setBorderColor(Color newColor) { borderColor = newColor; }
    void setTextColor(Color newColor) { textColor = newColor; }
    void setBorderThickness(float newThickness) { borderThickness = newThickness; }
    void setFontSize(int newFontSize) { fontSize = newFontSize; }

    // Input
    bool update(const Mouse& mouse) {
        pointerInside = mouse.isInside(rectangle);

        if (mouse.isLeftPressed() && pointerInside) {
            pressed = true;
        }

        if (mouse.isLeftReleased()) {
            bool clicked = pressed && pointerInside;
            pressed = false;

            return clicked;
        }

        return false;
    }

    // Drawing
    void draw() const {
        Color currentColor = normalColor;

        if (isPressed()) {
            currentColor = pressedColor;
        }
        else if (pointerInside) {
            currentColor = hoverColor;
        }

        DrawRectangleRec(rectangle, currentColor);
        DrawRectangleLinesEx(rectangle, borderThickness, borderColor);

        int textWidth = MeasureText(text.c_str(), fontSize);
        int textX = static_cast<int>(rectangle.x + (rectangle.width - static_cast<float>(textWidth)) / 2.0f);
        int textY = static_cast<int>(rectangle.y + (rectangle.height - static_cast<float>(fontSize)) / 2.0f);

        DrawText(text.c_str(), textX, textY, fontSize, textColor);
    }
};