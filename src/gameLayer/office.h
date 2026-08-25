#pragma once

#include <raylib.h>

#include "settings.h"
#include "spriteAtlas.h"

class OfficeSprite {
private:
    SpriteAtlas office;

    static constexpr float standardScale = 3.0f;

    static constexpr Vector2 doorPosition{ 352.0f, -30.0f };

    static constexpr Vector2 leftDeskPosition{ 302.0f, 300.0f };
    static constexpr Vector2 rightDeskPosition{ 402.0f, 300.0f };
    static constexpr Vector2 sideDeskPosition{ 650.0f, 170.0f };

    static constexpr Vector2 pencilPosition{ 407.0f, 280.0f };
    static constexpr Vector2 postItPosition{ 442.0f, 280.0f };
    static constexpr Vector2 penCupPosition{ 472.0f, 270.0f };
    static constexpr Vector2 cupPosition{ 422.0f, 285.0f };

    static constexpr Vector2 extinguisherPosition{ 500.0f, 80.0f };
    static constexpr Vector2 graphPosition{ 584.0f, 50.0f };
    static constexpr Vector2 printerPosition{ 650.0f, 150.0f };

public:
    void load() { office.load("officeAssets.png", 2, 9); }

    // Position ruler
    void drawPositionRuler() const {
        constexpr int spacing = 50;
        constexpr int majorSpacing = 100;
        constexpr int fontSize = 10;

        for (int x = 0; x < Settings::getVirtualWidth(); x += spacing) {
            bool majorLine = x % majorSpacing == 0;
            Color lineColor = majorLine ? Fade(YELLOW, 0.45f) : Fade(WHITE, 0.20f);

            DrawLine(x, 0, x, Settings::getVirtualHeight(), lineColor);
            DrawText(TextFormat("%d", x), x + 3, 3, fontSize, YELLOW);
        }

        for (int y = 0; y < Settings::getVirtualHeight(); y += spacing) {
            bool majorLine = y % majorSpacing == 0;
            Color lineColor = majorLine ? Fade(YELLOW, 0.45f) : Fade(WHITE, 0.20f);

            DrawLine(0, y, Settings::getVirtualWidth(), y, lineColor);
            DrawText(TextFormat("%d", y), 3, y + 3, fontSize, YELLOW);
        }
    }

    void drawPositionCoordinates(Vector2 position, Vector2 textOffset) const {
        constexpr int fontSize = 8;
        constexpr int padding = 2;

        const char* coordinateText = TextFormat("(%.0f, %.0f)", position.x, position.y);
        int textWidth = MeasureText(coordinateText, fontSize);

        Vector2 textPosition{ position.x + textOffset.x, position.y + textOffset.y };

        DrawLineV(position, textPosition, RED);
        DrawCircleV(position, 3.0f, RED);
        DrawRectangle(static_cast<int>(textPosition.x - padding), static_cast<int>(textPosition.y - padding), textWidth + padding * 2, fontSize + padding * 2, Fade(BLACK, 0.80f));
        DrawText(coordinateText, static_cast<int>(textPosition.x), static_cast<int>(textPosition.y), fontSize, YELLOW);
    }

    void drawOfficeCoordinates() const {
        drawPositionCoordinates(doorPosition, { 5.0f, 35.0f });

        drawPositionCoordinates(leftDeskPosition, { -45.0f, 40.0f });
        drawPositionCoordinates(rightDeskPosition, { 10.0f, 40.0f });
        drawPositionCoordinates(sideDeskPosition, { 10.0f, 40.0f });

        drawPositionCoordinates(pencilPosition, { -40.0f, -70.0f });
        drawPositionCoordinates(postItPosition, { -20.0f, -55.0f });
        drawPositionCoordinates(penCupPosition, { 15.0f, -40.0f });
        drawPositionCoordinates(cupPosition, { -45.0f, 20.0f });

        drawPositionCoordinates(extinguisherPosition, { 5.0f, -15.0f });
        drawPositionCoordinates(graphPosition, { 5.0f, -15.0f });
        drawPositionCoordinates(printerPosition, { 5.0f, -15.0f });
    }

    // Simple object drawing
    void drawDesk(Vector2 position) const { office.draw(0, 0, static_cast<int>(position.x), static_cast<int>(position.y), standardScale); }
    void drawPaper(Vector2 position) const { office.draw(0, 1, static_cast<int>(position.x), static_cast<int>(position.y), standardScale); }
    void drawPencil(Vector2 position) const { office.draw(0, 2, static_cast<int>(position.x), static_cast<int>(position.y), standardScale); }
    void drawPostIt(Vector2 position) const { office.draw(0, 3, static_cast<int>(position.x), static_cast<int>(position.y), standardScale); }
    void drawPenCup(Vector2 position) const { office.draw(0, 4, static_cast<int>(position.x), static_cast<int>(position.y), standardScale); }
    void drawPaperSheets(Vector2 position) const { office.draw(0, 5, static_cast<int>(position.x), static_cast<int>(position.y), standardScale); }
    void drawCup(Vector2 position) const { office.draw(1, 0, static_cast<int>(position.x), static_cast<int>(position.y), standardScale); }
    void drawPrinter(Vector2 position) const { office.draw(1, 1, static_cast<int>(position.x), static_cast<int>(position.y), standardScale); }
    void drawUnknownObject(Vector2 position) const { office.draw(1, 2, static_cast<int>(position.x), static_cast<int>(position.y), standardScale); }
    void drawScreen(Vector2 position) const { office.draw(1, 3, static_cast<int>(position.x), static_cast<int>(position.y), standardScale); }
    void drawExtinguisher(Vector2 position) const { office.draw(1, 4, static_cast<int>(position.x), static_cast<int>(position.y), standardScale); }
    void drawGraph(Vector2 position) const { office.draw(1, 5, static_cast<int>(position.x), static_cast<int>(position.y), standardScale); }
    void drawWall1(Vector2 position) const { office.draw(0, 6, static_cast<int>(position.x), static_cast<int>(position.y), standardScale); }
    void drawWall2(Vector2 position) const { office.draw(1, 6, static_cast<int>(position.x), static_cast<int>(position.y), standardScale); }
    void drawFloor1(Vector2 position) const { office.draw(0, 7, static_cast<int>(position.x), static_cast<int>(position.y), standardScale); }
    void drawFloor2(Vector2 position) const { office.draw(1, 7, static_cast<int>(position.x), static_cast<int>(position.y), standardScale); }

    // Complex object drawing
    void drawDoor(Vector2 position) const {
        float spriteHeight = office.getSpriteHeight() * standardScale;

        office.draw(0, 8, static_cast<int>(position.x), static_cast<int>(position.y), standardScale);
        office.draw(1, 8, static_cast<int>(position.x), static_cast<int>(position.y + spriteHeight), standardScale);
    }

    void drawFloor() const {
        int tileSize = static_cast<int>(office.getSpriteWidth() * standardScale);

        for (int y = 0; y < Settings::getVirtualHeight(); y += tileSize) {
            for (int x = 0; x < Settings::getVirtualWidth(); x += tileSize) {
                drawFloor1({ static_cast<float>(x), static_cast<float>(y) });
            }
        }
    }

    void drawWalls() const {
        int tileSize = static_cast<int>(office.getSpriteWidth() * standardScale);
        float firstRowY = 0.0f;
        float secondRowY = static_cast<float>(tileSize - 30);

        for (int x = 0; x < Settings::getVirtualWidth(); x += tileSize) {
            drawWall1({ static_cast<float>(x), firstRowY });
            drawWall1({ static_cast<float>(x), secondRowY });
        }
    }

    // Final drawing function
    void drawOffice() const {
        // Floor and walls
        drawFloor();
        drawWalls();

        // Door
        drawDoor(doorPosition);

        // Desks
        drawDesk(leftDeskPosition);
        drawDesk(rightDeskPosition);
        drawDesk(sideDeskPosition);

        // Document rendering is relegated to document.h

        // Desk objects
        //drawPencil(pencilPosition);
        //drawPostIt(postItPosition);
        //drawPenCup(penCupPosition);
        //drawCup(cupPosition);

        // Other objects
        drawExtinguisher(extinguisherPosition);
        drawGraph(graphPosition);
        drawPrinter(printerPosition);

        // Debug information
        //drawPositionRuler();
        //drawOfficeCoordinates();
    }
};