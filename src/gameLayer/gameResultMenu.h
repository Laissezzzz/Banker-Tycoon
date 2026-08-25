#pragma once

#include <raylib.h>

#include "button.h"
#include "mouse.h"
#include "game.h"
#include "player.h"
#include "loanApplicationManager.h"
#include "documentManager.h"

class GameResultMenu {
public:
    enum class Action { None, PreviousLoan, NextLoan, Restart, Exit };
    enum class View { Summary, LoanResult };

private:
    static constexpr int virtualWidth = 800;

    DocumentManager documentManager;

    Button seeResultsButton{ "See results", { 305.0f, 270.0f, 190.0f, 60.0f } };
    Button restartButton{ "Restart", { 190.0f, 350.0f, 190.0f, 60.0f } };
    Button exitButton{ "Exit", { 420.0f, 350.0f, 190.0f, 60.0f } };

    Button previousLoanButton{ "Previous loan", { 80.0f, 375.0f, 190.0f, 60.0f } };
    Button backButton{ "Go back", { 305.0f, 375.0f, 190.0f, 60.0f } };
    Button nextLoanButton{ "Next loan", { 530.0f, 375.0f, 190.0f, 60.0f } };

    Action action = Action::None;
    View view = View::Summary;

    void drawCenteredText(const char* text, int positionY, int fontSize, Color color) const {
        int textWidth = MeasureText(text, fontSize);
        int positionX = (virtualWidth - textWidth) / 2;

        DrawText(text, positionX, positionY, fontSize, color);
    }

    void updateSummary(const Mouse& mouse) {
        if (seeResultsButton.update(mouse)) {
            setViewToLoanResult();
            return;
        }

        if (restartButton.update(mouse) || IsKeyPressed(KEY_EIGHT)) {
            setActionToRestart();
            return;
        }

        if (exitButton.update(mouse) || IsKeyPressed(KEY_NINE)) {
            setActionToExit();
        }
    }

    void updateLoanResult(const Mouse& mouse) {
        if (previousLoanButton.update(mouse)) {
            setActionToPreviousLoan();
            return;
        }

        if (backButton.update(mouse)) {
            setViewToSummary();
            return;
        }

        if (nextLoanButton.update(mouse)) {
            setActionToNextLoan();
        }
    }

    void drawSummary(const Game& game, const Player& player, const LoanApplicationManager& loanApplicationManager) const {
        const char* resultText = game.hasWon() ? "YOU WON!" : "YOU LOST!";
        Color resultColor = game.hasWon() ? GREEN : RED;

        drawCenteredText(resultText, 70, 50, resultColor);
        drawCenteredText(TextFormat("Banker: %s", player.getName().c_str()), 150, 25, WHITE);
        drawCenteredText(TextFormat("Final budget: %.2f", loanApplicationManager.getCurrentBudget()), 190, 25, WHITE);
        drawCenteredText(TextFormat("Target budget: %.2f", game.getTargetBudget()), 225, 25, WHITE);

        seeResultsButton.draw();
        restartButton.draw();
        exitButton.draw();
    }

    void drawLoanResult(const LoanApplicationManager& loanApplicationManager) const {
        if (!loanApplicationManager.hasLoanResults()) {
            drawCenteredText("No loan results", 180, 30, WHITE);
            backButton.draw();
            return;
        }

        drawCenteredText(TextFormat("Loan result: %d/%d", loanApplicationManager.getResultIndex() + 1, loanApplicationManager.getLoanResultCount()), 40, 30, WHITE);

        documentManager.drawLoanOutcome(loanApplicationManager.getCurrentLoanResult());

        previousLoanButton.draw();
        backButton.draw();
        nextLoanButton.draw();
    }

public:
    // Getter functions
    Action getAction() const { return action; }
    View getView() const { return view; }

    bool isPreviousLoanRequested() const { return action == Action::PreviousLoan; }
    bool isNextLoanRequested() const { return action == Action::NextLoan; }
    bool isRestartRequested() const { return action == Action::Restart; }
    bool isExitRequested() const { return action == Action::Exit; }

    bool isSummaryView() const { return view == View::Summary; }
    bool isLoanResultView() const { return view == View::LoanResult; }

    // Setter functions
    void setAction(Action newAction) { action = newAction; }
    void setActionToNone() { setAction(Action::None); }
    void setActionToPreviousLoan() { setAction(Action::PreviousLoan); }
    void setActionToNextLoan() { setAction(Action::NextLoan); }
    void setActionToRestart() { setAction(Action::Restart); }
    void setActionToExit() { setAction(Action::Exit); }

    void setView(View newView) { view = newView; }
    void setViewToSummary() { setView(View::Summary); }
    void setViewToLoanResult() { setView(View::LoanResult); }

    // Update
    void updateLoanResult(const Mouse& mouse, LoanApplicationManager& loanApplicationManager) {
        if (previousLoanButton.update(mouse)) {
            loanApplicationManager.showPreviousLoanResult();
            return;
        }

        if (backButton.update(mouse)) {
            setViewToSummary();
            return;
        }

        if (nextLoanButton.update(mouse)) {
            loanApplicationManager.showNextLoanResult();
        }
    }

    void update(const Mouse& mouse, LoanApplicationManager& loanApplicationManager) {
        setActionToNone();

        if (isSummaryView()) {
            updateSummary(mouse);
            return;
        }

        updateLoanResult(mouse, loanApplicationManager);
    }

    // Drawing
    void draw(const Game& game, const Player& player, const LoanApplicationManager& loanApplicationManager) const {
        ClearBackground(BLACK);

        if (isSummaryView()) {
            drawSummary(game, player, loanApplicationManager);
            return;
        }

        drawLoanResult(loanApplicationManager);
    }

};