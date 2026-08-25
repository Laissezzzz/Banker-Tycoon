#pragma once

#include <raylib.h>
#include <string>

#include "game.h"
#include "mouse.h"
#include "button.h"

class StartingMenu {
private:
    std::string playerNameInput = "";
    static constexpr std::size_t maximumNameSize = 10;

    Button confirmButton{ "Save name and start", { 250.0f, 370.0f, 300.0f, 50.0f } };

    bool invalidName = false;
    bool menuCompleted = false;
    bool playerNameLoaded = false;

    void updateNameInput() {
        int character = GetCharPressed();

        while (character > 0) {
            if (character >= 32 && character <= 125 && playerNameInput.size() < maximumNameSize) {
                playerNameInput += static_cast<char>(character);
                invalidName = false;
            }

            character = GetCharPressed();
        }

        if (IsKeyPressed(KEY_BACKSPACE) && !playerNameInput.empty()) {
            playerNameInput.pop_back();
            invalidName = false;
        }
    }

public:
    // Getter functions
    const std::string& getPlayerNameInput() const { return playerNameInput; }
    bool isPlayerNameValid(const Game& game) const { return game.getPlayer().isNameValid(playerNameInput); }
    bool getMenuCompleted() const { return menuCompleted; }

    // Setter functions
    void setName(const std::string& newName) { playerNameInput = newName; }

    void setNameFromGame(const Game& game) {
        setName(game.getPlayer().getName());
        playerNameLoaded = true;
    }

    // Name confirmation
    bool confirmName(Game& game) {
        if (!isPlayerNameValid(game)) {
            invalidName = true;
            return false;
        }

        game.getPlayer().setName(playerNameInput);
        invalidName = false;
        menuCompleted = true;

        return true;
    }

    // Update
    void update(Game& game, const Mouse& mouse) {
        if (!playerNameLoaded) {
            setNameFromGame(game);
        }

        updateNameInput();

        bool buttonClicked = confirmButton.update(mouse);

        if (buttonClicked) {
            confirmName(game);
        }
    }

    // Drawing
    void draw(const Game& game) const {
        ClearBackground(BLACK);

        DrawText("What's your name?", 100, 50, 30, WHITE);

        DrawRectangle(100, 100, 300, 50, RAYWHITE);
        DrawRectangleLines(100, 100, 300, 50, BLACK);
        DrawText(playerNameInput.c_str(), 110, 115, 20, BLACK);

        if (invalidName) {
            DrawText("The name is invalid", 100, 155, 20, RED);
        }

        DrawText(game.getRules().c_str(), 50, 200, 20, WHITE);

        confirmButton.draw();
    }

    void reset() {
        playerNameInput.clear();
        invalidName = false;
        menuCompleted = false;
        playerNameLoaded = false;
    }
};