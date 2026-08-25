#pragma once

#include <raylib.h>
#include <cmath>

#include "loanApplicationManager.h"
#include "game.h"
#include "player.h"

class Hud {
private:
    Vector2 position{20.0f,10.0f };
    Vector2 size{ 270.0f, 160.0f };
    Vector2 padding{ 20.0f, 20.0f };

    Color backgroundColor = WHITE;
    Color borderColor = BLACK;

    float borderThickness = 2.0f;
    int fontSize = 20;
    int lineSpacing = 25;

    Rectangle getBackgroundRectangle() const {return {position.x,position.y, size.x, size.y};}
    Vector2 getTextPosition(int line) const {return {position.x + padding.x,position.y + padding.y + lineSpacing * line};}

    void drawTextLine(const char* text, int line, Color color) const {
        Vector2 textPosition = getTextPosition(line);

        DrawText(text, static_cast<int>(textPosition.x), static_cast<int>(textPosition.y), fontSize, color);
    }

public:
    // Getter functions
    Vector2 getPosition() const { return position; }
    Vector2 getSize() const { return size; }

    // Setter functions
    void setPosition(Vector2 newPosition) { position = newPosition; }
    void setSize(Vector2 newSize) { size = newSize; }

    void drawHud(const LoanApplicationManager& loanApplicationManager, const Game& game, const Player& player) const {
        Rectangle background = getBackgroundRectangle();

        DrawRectangleRec(background, backgroundColor);
        DrawRectangleLinesEx(background, borderThickness, borderColor);

        int loanSize = loanApplicationManager.getLoanSize();
        int currentLoanNumber = loanApplicationManager.getIndex() + 1;
        int currentLoanId = loanApplicationManager.getCurrentLoanId();
        int remainingTime = static_cast<int>(std::ceil(game.getRemainingTimeSeconds()));

        if (currentLoanNumber > loanSize) {
            currentLoanNumber = loanSize;
        }

        if (loanSize == 0) {
            currentLoanNumber = 0;
        }

        drawTextLine(TextFormat("Your name: %s", player.getName().c_str()), 0, BLACK);
        drawTextLine(TextFormat("Client n.: %i/%i", currentLoanNumber, loanSize), 1, BLACK);
        drawTextLine(TextFormat("Budget left: %.2f", loanApplicationManager.getCurrentBudget()), 2, BLACK);
        drawTextLine(TextFormat("Seconds left: %i", remainingTime), 3, BLACK);

        if (currentLoanId == BankDB::invalidId) {
            drawTextLine("All proposals examined", 4, DARKGREEN);
            return;
        }

        drawTextLine(TextFormat("Status: %s", Loan::loanStatusToString(loanApplicationManager.getCurrentLoan().getStatus())), 4, BLACK);
    }
};