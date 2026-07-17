#pragma once

#include "spriteAtlas.h"
#include <string>

class Document {
private:
    SpriteAtlas document{};
    std::string name="Blank";
    std::string surname="Blank";
    std::string civilStatus = "Single";
    std::string birthDate = "01/01/2000";
    int childNumber = 0;

public:
    void load() {
        document.load("id.png", 1, 1);
    }

    void draw(int posX, int posY) const {
        const float scale = 0.3f;
        document.draw(0, 0, posX, posY, scale);
        DrawText(name.c_str(), posX + 20, posY - 70, 16, BLACK);
        DrawText(surname.c_str(), posX + 20, posY - 40, 16, BLACK);
        DrawText(birthDate.c_str(), posX + 30, posY , 20, BLACK);
        DrawText(civilStatus.c_str(), posX + 20, posY + 30, 16, BLACK);
        DrawText(TextFormat("%d", childNumber), posX + 30, posY + 60, 16, BLACK);
    }
};