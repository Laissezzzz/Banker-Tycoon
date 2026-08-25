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

        SetTextureFilter(texture, TEXTURE_FILTER_POINT);

        spriteWidth = texture.width / columns;
        spriteHeight = texture.height / rows;

        TraceLog(LOG_INFO, "Texture ID: %u", texture.id);
        TraceLog(LOG_INFO, "Texture size: %d x %d", texture.width, texture.height);
    }

    Rectangle getSprite(int row, int column) const {
        if (row < 0 || row >= rows || column < 0 || column >= columns) {
            TraceLog(LOG_ERROR, "Sprite position out of atlas bounds");
            return Rectangle{};
        }

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
            static_cast<float>(spriteWidth) * scale,
            static_cast<float>(spriteHeight) * scale
        };
    }

    void draw(int row, int column, int posX, int posY, float scale = 1.0f) const {
        const Rectangle source = getSprite(row, column);
        const Rectangle destination = getDestination(posX, posY, scale);

        DrawTexturePro(texture, source, destination, { 0.0f, 0.0f }, 0.0f, WHITE);
    }

    // Getter functions
    int getSpriteWidth() const { return spriteWidth; }
    int getSpriteHeight() const { return spriteHeight; }
    int getSpriteLength() const { return spriteWidth * spriteHeight / 2; }
};