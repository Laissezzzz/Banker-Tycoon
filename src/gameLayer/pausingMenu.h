#pragma once

#include <raylib.h>

#include "button.h"
#include "mouse.h"

class PausingMenu {
public:
    enum class Action { None, Pause, Continue, Restart };

private:
    Button continueButton{ "Continue", { 300.0f, 170.0f, 200.0f, 60.0f } };
    Button restartButton{ "Restart", { 300.0f, 250.0f, 200.0f, 60.0f } };
    Button pauseButton{ "Pause", { 680.0f, 20.0f, 100.0f, 45.0f } };

    Action action = Action::None;

public:
    // Getter functions
    Action getAction() const { return action; }
    bool isPauseRequested() const { return action == Action::Pause; }
    bool isContinueRequested() const { return action == Action::Continue; }
    bool isRestartRequested() const { return action == Action::Restart; }

    // Setter functions
    void setAction(Action newAction) { action = newAction; }
    void setActionToNone() { setAction(Action::None); }
    void setActionToPause() { setAction(Action::Pause); }
    void setActionToContinue() { setAction(Action::Continue); }
    void setActionToRestart() { setAction(Action::Restart); }

    // Pause button update
    void updatePauseButton(const Mouse& mouse) {
        setActionToNone();

        if (pauseButton.update(mouse)) {
            setActionToPause();
        }
    }

    // Pausing menu update
    void update(const Mouse& mouse) {
        setActionToNone();

        if (continueButton.update(mouse)) {
            setActionToContinue();
            return;
        }

        if (restartButton.update(mouse)) {
            setActionToRestart();
        }
    }

    // Drawing
    void drawPauseButton() const { pauseButton.draw(); }

    void draw() const {
        ClearBackground(BLACK);

        DrawText("GAME PAUSED", 250, 80, 40, WHITE);

        continueButton.draw();
        restartButton.draw();
    }
};