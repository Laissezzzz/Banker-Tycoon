#pragma once

#include <raylib.h>
#include <string>

class SpriteAtlas {
private:
    std::string name;
    Texture2D texture{};

    int rows = 0;
    int columns = 0;
    int spriteWidth = 0;
    int spriteHeight = 0;

public:
    SpriteAtlas() = default;

    void load(const std::string& newName, int newRows, int newColumns) {
        name = newName;
        rows = newRows;
        columns = newColumns;

        texture = LoadTexture((std::string(RESOURCES_PATH) + name).c_str());

        spriteWidth = texture.width / columns;
        spriteHeight = texture.height / rows;
        TraceLog(LOG_INFO, "Texture ID: %u", texture.id);
        TraceLog(LOG_INFO, "Texture size: %d x %d", texture.width, texture.height);
    }

    Rectangle getSprite(int row, int column) const {
        return Rectangle{
            static_cast<float>(column * spriteWidth),
            static_cast<float>(row * spriteHeight),
            static_cast<float>(spriteWidth),
            static_cast<float>(spriteHeight)
        };
    }

    Rectangle getDestination(int posX, int posY, float scale) const {
        return Rectangle{
            static_cast<float>(posX),
            static_cast<float>(posY),
            static_cast<float>(spriteWidth*scale),
            static_cast<float>(spriteHeight*scale)
        };
    }

    Vector2 getCentralOrigin(const Rectangle& destination) const {
        return Vector2{
            destination.width / 2.0f,
            destination.height / 2.0f
        };
    }

    void draw(int row, int column, int posX, int posY, float scale=1.0) const {
        const Rectangle source = getSprite(row, column);
        const Rectangle destination = getDestination(posX, posY,scale);
        const Vector2 origin = getCentralOrigin(destination);;

        DrawTexturePro(texture, source, destination, origin, 0.0f, WHITE);
    }
};