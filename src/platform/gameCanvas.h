#pragma once

#include <raylib.h>
#include <algorithm>

#include "settings.h"

class GameCanvas {
private:
    RenderTexture2D canvas{};

    static constexpr float desktopMargin = 20.0f;
    static constexpr float webMargin = 2.0f;
    static constexpr float lineThickness = 2.0f;
    static constexpr float rotation = 0.0f;

    Color backgroundTint = DARKGRAY;

    float getEffectiveMargin() const {
#if defined(PLATFORM_WEB)
        return webMargin;
#else
        const float smallestScreenSide = static_cast<float>(
            std::min(GetScreenWidth(), GetScreenHeight())
            );

        const float largestSafeMargin = std::max(
            0.0f,
            (smallestScreenSide - 1.0f) * 0.5f
        );

        return std::min(desktopMargin, largestSafeMargin);
#endif
    }

public:
    void setUp() {
        canvas = LoadRenderTexture(
            Settings::getVirtualWidth(),
            Settings::getVirtualHeight()
        );

        SetTextureFilter(canvas.texture, TEXTURE_FILTER_POINT);
    }

    void begin() const {
        BeginTextureMode(canvas);
        ClearBackground(backgroundTint);
    }

    void end() const {
        EndTextureMode();
    }

    Rectangle getDestinationRectangle() const {
        const float screenWidth = static_cast<float>(
            std::max(1, GetScreenWidth())
            );
        const float screenHeight = static_cast<float>(
            std::max(1, GetScreenHeight())
            );
        const float margin = getEffectiveMargin();

        const float availableWidth = std::max(
            1.0f,
            screenWidth - margin * 2.0f
        );
        const float availableHeight = std::max(
            1.0f,
            screenHeight - margin * 2.0f
        );

        const float scaleX = availableWidth /
            static_cast<float>(Settings::getVirtualWidth());
        const float scaleY = availableHeight /
            static_cast<float>(Settings::getVirtualHeight());
        const float scale = std::max(0.001f, std::min(scaleX, scaleY));

        const float width =
            static_cast<float>(Settings::getVirtualWidth()) * scale;
        const float height =
            static_cast<float>(Settings::getVirtualHeight()) * scale;

        return {
            (screenWidth - width) * 0.5f,
            (screenHeight - height) * 0.5f,
            width,
            height
        };
    }

    void draw() const {
        const Rectangle source{
            0.0f,
            0.0f,
            static_cast<float>(Settings::getVirtualWidth()),
            -static_cast<float>(Settings::getVirtualHeight())
        };
        const Rectangle destination = getDestinationRectangle();

        DrawTexturePro(
            canvas.texture,
            source,
            destination,
            Vector2{},
            rotation,
            WHITE
        );

        DrawRectangleLinesEx(destination, lineThickness, WHITE);
    }

    void close() {
        if (canvas.id != 0) {
            UnloadRenderTexture(canvas);
            canvas = {};
        }
    }

    const RenderTexture2D& getCanvas() const {
        return canvas;
    }

    int getCanvasWidth() const {
        return Settings::getVirtualWidth();
    }

    int getCanvasHeight() const {
        return Settings::getVirtualHeight();
    }

    float getScale() const {
        return getDestinationRectangle().width /
            static_cast<float>(Settings::getVirtualWidth());
    }

    Vector2 getCanvasOffset() const {
        const Rectangle destination = getDestinationRectangle();
        return { destination.x, destination.y };
    }

    Vector2 getCanvasPosition(Vector2 screenPosition) const {
        const Rectangle destination = getDestinationRectangle();
        const float scale = destination.width /
            static_cast<float>(Settings::getVirtualWidth());

        return {
            (screenPosition.x - destination.x) / scale,
            (screenPosition.y - destination.y) / scale
        };
    }

    bool isCanvasPositionInside(Vector2 canvasPosition) const {
        return canvasPosition.x >= 0.0f &&
            canvasPosition.y >= 0.0f &&
            canvasPosition.x < static_cast<float>(Settings::getVirtualWidth()) &&
            canvasPosition.y < static_cast<float>(Settings::getVirtualHeight());
    }

    bool isScreenPositionInside(Vector2 screenPosition) const {
        return CheckCollisionPointRec(
            screenPosition,
            getDestinationRectangle()
        );
    }

    Vector2 getMousePosition() const {
        return getCanvasPosition(::GetMousePosition());
    }

    bool isMouseInside() const {
        return isScreenPositionInside(::GetMousePosition());
    }
};
