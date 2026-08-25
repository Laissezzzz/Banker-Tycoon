#pragma once

#include <raylib.h>
#include <algorithm>
#include "settings.h"

class GameCanvas {
	RenderTexture2D canvas{};
	Color backgroundTint = DARKGRAY;
	float margin = 20.0f;
	float lineThickness = 2.0f;
	float rotation = 0.0f;
public:
	void setUp() {
		canvas = LoadRenderTexture(Settings::getVirtualWidth(), Settings::getVirtualHeight());
		SetTextureFilter(canvas.texture, TEXTURE_FILTER_POINT);
	}

	void begin() const {
		BeginTextureMode(canvas);
		ClearBackground(backgroundTint);
	}

	void end() const {
		EndTextureMode();
	}

	void draw() const {
		const float availableWidth = GetScreenWidth() - margin * 2.0f;
		const float availableHeight = GetScreenHeight() - margin * 2.0f;

		const float scaleX = availableWidth / static_cast<float>(Settings::getVirtualWidth());
		const float scaleY = availableHeight / static_cast<float>(Settings::getVirtualHeight());
		const float scale = std::min(scaleX, scaleY);

		const float width = Settings::getVirtualWidth() * scale;
		const float height = Settings::getVirtualHeight() * scale;

		const float offsetX = (GetScreenWidth() - width) / 2.0f;
		const float offsetY = (GetScreenHeight() - height) / 2.0f;

		const Rectangle source{ 0.0f, 0.0f, static_cast<float>(Settings::getVirtualWidth()), -static_cast<float>(Settings::getVirtualHeight()) };
		const Rectangle destination{ offsetX, offsetY, width, height };

		DrawTexturePro(canvas.texture, source, destination, Vector2{}, rotation, WHITE);
		DrawRectangleLinesEx(destination, lineThickness, WHITE);
	}

	void close() {
		UnloadRenderTexture(canvas);
	}

	//Getter functions
	const RenderTexture2D& getCanvas() const {return canvas;}
	int getCanvasWidth() const {return Settings::getVirtualWidth();	}
	int getCanvasHeight() const {return Settings::getVirtualHeight();}

	float getScale() const {
		const float availableWidth = GetScreenWidth() - margin * 2.0f;
		const float availableHeight = GetScreenHeight() - margin * 2.0f;

		const float scaleX = availableWidth / static_cast<float>(Settings::getVirtualWidth());
		const float scaleY = availableHeight / static_cast<float>(Settings::getVirtualHeight());

		return std::min(scaleX, scaleY);
	}

	Vector2 getCanvasOffset() const {
		const float scale = getScale();

		const float width = Settings::getVirtualWidth() * scale;
		const float height = Settings::getVirtualHeight() * scale;

		return {
			(GetScreenWidth() - width) / 2.0f,
			(GetScreenHeight() - height) / 2.0f
		};
	}

	Vector2 getMousePosition() const {
		const Vector2 mousePos = ::GetMousePosition();
		const Vector2 offset = getCanvasOffset();
		const float scale = getScale();

		return {
			(mousePos.x - offset.x) / scale,
			(mousePos.y - offset.y) / scale
		};
	}

	bool isMouseInside() const {
		const Vector2 mousePos = getMousePosition();

		return mousePos.x >= 0.0f &&
			mousePos.y >= 0.0f &&
			mousePos.x < Settings::getVirtualWidth() &&
			mousePos.y < Settings::getVirtualHeight();
	}
};