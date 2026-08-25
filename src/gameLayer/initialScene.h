#pragma once

#include <raylib.h>

#include "game.h"
#include "mouse.h"
#include "button.h"

class InitialScene {
private:
    float timer = 0.0f;
    static constexpr float buttonDelay = 0.0f;

    Button continueButton{ "Main menu", { 300.0f, 320.0f, 200.0f, 60.0f } };

public:
    bool draw(Game& game, const Mouse& mouse) {
        timer += GetFrameTime();

        ClearBackground(BLACK);

        DrawText("BANKER TYCOON", 220, 170, 50, WHITE);
        DrawText("A game about banking decisions", 235, 235, 20, GRAY);

        if (timer >= buttonDelay) {
            if (continueButton.update(mouse)) {
                reset();
                game.setGamePhaseToStartingMenu();
            }

            continueButton.draw();
        }

        return true;
    }

    void setButtonPosition(Vector2 newPosition) {
        continueButton.setPosition(newPosition);
    }

    void reset() {
        timer = 0.0f;
    }
};