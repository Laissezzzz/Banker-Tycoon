#pragma once

#include <string>

#include "player.h"

class Game {
private:
    enum class GamePhase { InitialScene, StartingMenu, LoanExamination, PausingMenu, GameResult };
    enum class Language {English, Italian};

    Player player;

    static constexpr double startingBudget      = 500000.0;

    static constexpr double targetBudget        = 550000.0;
    static constexpr float gameDurationSeconds  = 600.0f;
    static constexpr int loanApplicationCount   = 5;

    GamePhase phase                             = GamePhase::InitialScene;
    Language language                           = Language::Italian;

    std::string rules = "Examine every loan application and decide whether to approve or reject it. Reach the target budget before the time expires.";

    bool victory                                = false;
    float remainingTimeSeconds                  = gameDurationSeconds;

public:
    // Game phase getters
    bool isGamePhaseInitialScene() const { return phase == GamePhase::InitialScene; }
    bool isGamePhaseStartingMenu() const { return phase == GamePhase::StartingMenu; }
    bool isGamePhaseLoanExamination() const { return phase == GamePhase::LoanExamination; }
    bool isGamePhasePausingMenu() const { return phase == GamePhase::PausingMenu; }
    bool isGamePhaseGameResult() const { return phase == GamePhase::GameResult; }
    bool isGameInEnglish() const { return language == Language::English; }
    bool isGameInItalian() const { return language == Language::Italian; }

    // Game phase setters
    void setGamePhaseToInitialScene() { phase = GamePhase::InitialScene; }
    void setGamePhaseToStartingMenu() { phase = GamePhase::StartingMenu; }
    void setGamePhaseToLoanExamination() { phase = GamePhase::LoanExamination; }
    void setGamePhaseToPausingMenu() { phase = GamePhase::PausingMenu; }
    void setGamePhaseToGameResult() { phase = GamePhase::GameResult; }
    void setGameLanguageToItalian() { language = Language::Italian; }
    void setGameLanguageToEnglish() { language = Language::English; }

    // Player
    Player& getPlayer() { return player; }
    const Player& getPlayer() const { return player; }

    // Rules
    const std::string& getRules() const { 
        return rules; }

    // Game values
    double getStartingBudget() const { return startingBudget; }
    double getTargetBudget() const { return targetBudget; }
    float getGameDurationSeconds() const { return gameDurationSeconds; }
    float getRemainingTimeSeconds() const { return remainingTimeSeconds; }
    int getLoanApplicationCount() const { return loanApplicationCount; }
    bool hasWon() const { return victory; }
    bool hasTimeExpired() const { return remainingTimeSeconds <= 0.0f; }

    // Timer
    void updateTimer(float deltaTime) {
        if (phase != GamePhase::LoanExamination) {
            return;
        }

        remainingTimeSeconds -= deltaTime;

        if (remainingTimeSeconds <= 0.0f) {
            remainingTimeSeconds = 0.0f;
        }
    }

    // Game result
    void setGameResult(double currentBudget) {
        victory = currentBudget >= targetBudget;
        phase = GamePhase::GameResult;
    }

    // Game reset
    void resetGame() {
        phase = GamePhase::StartingMenu;
        victory = false;
        remainingTimeSeconds = gameDurationSeconds;
    }
};