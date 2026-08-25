#include <raylib.h>

#include "settings.h"
#include "gameMain.h"
#include "deltaTime.h"
#include "bankDb.h"
#include "loanApplicationManager.h"
#include "game.h"
#include "initialScene.h"
#include "startingMenu.h"
#include "pausingMenu.h"
#include "loanExamination.h"
#include "gameResultMenu.h"
#include "mouse.h"

// Game objects
Game game;
DeltaTime deltaTime;
BankDB bankDb;
LoanApplicationManager loanApplicationManager;

// Mouse is created and updated in main.cpp.

// Scene objects
InitialScene initialSceneObject;
StartingMenu startingMenuObject;
PausingMenu pausingMenuObject;
LoanExamination loanExaminationObject;
GameResultMenu gameResultMenuObject;

bool initGame() {
    loanExaminationObject.initialize();

    if (!bankDb.initialize()) {
        return false;
    }

    if (!bankDb.resetLoanStatuses()) {
        bankDb.close();
        return false;
    }

    loanApplicationManager.initialize(bankDb, game);

    return true;
}

bool initialScene(const Mouse& mouse) {
    return initialSceneObject.draw(game, mouse);
}

bool startingMenu(const Mouse& mouse) {
    startingMenuObject.update(game, mouse);

    if (startingMenuObject.getMenuCompleted()) {
        game.setGamePhaseToLoanExamination();
    }

    startingMenuObject.draw(game);

    return true;
}

bool pausingMenu(const Mouse& mouse) {
    pausingMenuObject.update(mouse);

    if (pausingMenuObject.isContinueRequested()) {
        game.setGamePhaseToLoanExamination();
    }

    if (pausingMenuObject.isRestartRequested()) {
        game.resetGame();
        startingMenuObject.reset();
        loanExaminationObject.reset();

        if (!bankDb.resetLoanStatuses()) {
            return false;
        }

        loanApplicationManager.initialize(bankDb, game);
    }

    pausingMenuObject.draw();

    return true;
}

bool loanExamination(const Mouse& mouse) {
    // Logic
    loanExaminationObject.update(mouse, loanApplicationManager, bankDb);
    game.updateTimer(deltaTime.getDeltaTime());
    pausingMenuObject.updatePauseButton(mouse);

    if (loanExaminationObject.isFinished(game)) {
        loanApplicationManager.seeResult(game, bankDb);
        game.setGamePhaseToGameResult();
    }
    else if (pausingMenuObject.isPauseRequested()) {
        game.setGamePhaseToPausingMenu();
    }

    // Drawing
    loanExaminationObject.draw(loanApplicationManager, game);
    pausingMenuObject.drawPauseButton();

    return true;
}

bool gameResult(const Mouse& mouse) {
    gameResultMenuObject.update(mouse, loanApplicationManager);

    if (gameResultMenuObject.isRestartRequested()) {
        game.resetGame();
        startingMenuObject.reset();
        loanExaminationObject.reset();

        if (!bankDb.resetLoanStatuses()) {
            return false;
        }

        loanApplicationManager.initialize(bankDb, game);
    }

    if (gameResultMenuObject.isExitRequested()) {
        return false;
    }

    gameResultMenuObject.draw(game, game.getPlayer(), loanApplicationManager);

    return true;
}

bool updateGame(const Mouse& mouse) {
    deltaTime.update();

    bool running = false;

    if (game.isGamePhaseInitialScene()) {
        running = initialScene(mouse);
    }
    else if (game.isGamePhaseStartingMenu()) {
        running = startingMenu(mouse);
    }
    else if (game.isGamePhaseLoanExamination()) {
        running = loanExamination(mouse);
    }
    else if (game.isGamePhasePausingMenu()) {
        running = pausingMenu(mouse);
    }
    else if (game.isGamePhaseGameResult()) {
        running = gameResult(mouse);
    }
    else {
        return false;
    }

    //mouse.drawDebug();

    return running;
}

void closeGame() {
    bankDb.close();
}