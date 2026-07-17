#pragma once

#include <raylib.h>
#include <settings.h>
#include "spriteAtlas.h"

class OfficeSprite {
private:
    SpriteAtlas office{};
    const float standardScale = 3.0f;

public:
    void load() {
        office.load("officeAssets.png", 2, 9);
    }

    void drawDesk(int posX, int posY) const {
        const float scale = standardScale;
        office.draw(0, 0, posX, posY, scale);
    }

    void drawPaper(int posX, int posY) const {
        const float scale = standardScale;
        office.draw(0, 1, posX, posY, scale);
    }

    void drawPencil(int posX, int posY) const {
        const float scale = standardScale;
        office.draw(0, 2, posX, posY, scale);
    }

    void drawPostIt(int posX, int posY) const {
        const float scale = standardScale;
        office.draw(0, 3, posX, posY, scale);
    }

    void drawPenCup(int posX, int posY) const {
        const float scale = standardScale;
        office.draw(0, 4, posX, posY, scale);
    }

    void drawPaperSheets(int posX, int posY) const {
        const float scale = standardScale;
        office.draw(0, 5, posX, posY, scale);
    }

    void drawCup(int posX, int posY) const {
        const float scale = standardScale;
        office.draw(1, 0, posX, posY, scale);
    }

    void drawPrinter(int posX, int posY) const {
        const float scale = standardScale;
        office.draw(1, 1, posX, posY, scale);
    }

    void drawUnknownObject(int posX, int posY) const {
        const float scale = standardScale;
        office.draw(1, 2, posX, posY, scale);
    }

    void drawScreen(int posX, int posY) const {
        const float scale = standardScale;
        office.draw(1, 3, posX, posY, scale);
    }

    void drawExtinguisher(int posX, int posY) const {
        const float scale = standardScale;
        office.draw(1, 4, posX, posY, scale);
    }

    void drawGraph(int posX, int posY) const {
        const float scale = standardScale;
        office.draw(1, 5, posX, posY, scale);
    }

    void drawWall1(int posX, int posY) const {
        const float scale = standardScale;
        office.draw(0, 6, posX, posY, scale);
    }

    void drawWall2(int posX, int posY) const {
        const float scale = standardScale;
        office.draw(1, 6, posX, posY, scale);
    }

    void drawFloor1(int posX, int posY) const {
        const float scale = standardScale;
        office.draw(0, 7, posX, posY, scale);
    }

    void drawFloor2(int posX, int posY) const {
        const float scale = standardScale;
        office.draw(1, 7, posX, posY, scale);
    }

    void drawDoor(int posX, int posY) const {
        const float scale = standardScale;
        const float spriteHeight = office.getSpriteHeight() * scale;

        office.draw(0, 8, posX, posY, scale);
        office.draw(1, 8, posX, posY + static_cast<int>(spriteHeight), scale);
    }

    void drawFloor() const {
        const float scale = standardScale;
        const int tileSize = static_cast<int>(office.getSpriteWidth() * scale);

        for (int y = tileSize / 2; y < Settings::getVirtualHeight() + tileSize; y += tileSize) {
            for (int x = tileSize / 2; x < Settings::getVirtualWidth() + tileSize; x += tileSize) {
                drawFloor1(x, y);
            }
        }
    }

    void drawWalls() const {
        const float scale = standardScale;
        const int tileSize = static_cast<int>(office.getSpriteWidth() * scale);
        const int firstRowY = tileSize / 2;
        const int secondRowY = firstRowY + tileSize;

        for (int x = tileSize / 2; x < Settings::getVirtualWidth() + tileSize; x += tileSize) {
            drawWall1(x, firstRowY);
            drawWall1(x, secondRowY-30);
        }
    }

    //Final function
    void drawOffice() const {
        // Pavimento
        drawFloor();

        // Parete superiore
        drawWalls();

        // Porta centrale
        drawDoor(400, 20);

        // Scrivania principale
        drawDesk(400, 300);

        // Oggetti sulla scrivania
        drawScreen(400, 245);
        drawPaperSheets(340, 285);
        drawPencil(455, 280);
        drawPostIt(490, 280);
        drawPenCup(520, 270);
        drawCup(470, 285);

        // Altri elementi
        drawPrinter(650, 100);
        drawExtinguisher(100, 100);
        drawGraph(650,80);
    }
};