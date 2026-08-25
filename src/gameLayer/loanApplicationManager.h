#pragma once

#include <vector>

#include "bankDb.h"
#include "loanApplication.h"
#include "creditReport.h"
#include "client.h"
#include "employmentSituation.h"
#include "financialSituation.h"
#include "internalBehavior.h"
#include "loan.h"
#include "game.h"

class LoanApplicationManager {
    /// <summary>
    /// API RULES:
    ///     1-Any functions that uses the database need to call upon it in the function call.
    /// </summary>
private:
    LoanApplication loanApplication{};

    std::vector<int> loanSet{};
    int index = 0;

    double currentBudget = 0.0;

    std::vector<Loan> loanResults{};
    int resultIndex = 0;

public:

    // Getter functions
    LoanApplication& getLoanApplication() { return loanApplication; }
    const LoanApplication& getLoanApplication() const { return loanApplication; }
    const Client& getClient() const { return loanApplication.getClient(); }
    const CreditReport& getCreditReport() const { return loanApplication.getCreditReport(); }
    const FinancialSituation& getFinancialSituation() const { return loanApplication.getFinancialSituation(); }
    const EmploymentSituation& getEmploymentSituation() const { return loanApplication.getEmploymentSituation(); }
    const InternalBehavior& getInternalBehavior() const { return loanApplication.getInternalBehavior(); }
    Loan& getCurrentLoan() {return loanApplication.getLoan();}
    const Loan& getCurrentLoan() const {return loanApplication.getLoan();}
    const std::vector<int>& getLoanSet() const { return loanSet; }
    int getIndex() const { return index; }
    int getLoanSize() const {return static_cast<int>(loanSet.size());}
    double getCurrentBudget() const { return currentBudget; }const Loan& getCurrentLoanResult() const { return loanResults[resultIndex]; }
    int getResultIndex() const { return resultIndex; }
    int getLoanResultCount() const { return static_cast<int>(loanResults.size()); }
    bool hasLoanResults() const { return !loanResults.empty(); }
    int getCurrentLoanId() const {
        if (index < 0 || index >= static_cast<int>(loanSet.size())) {
            return BankDB::invalidId;
        }

        return loanSet[index];
    }

    // Setter functions
    void setIndex(int n) { index = n; }
    void setIndexToZero() { setIndex(0); }
    void setCurrentBudget(double n) { currentBudget = n; }
    void setCurrentLoanToApproved() { getCurrentLoan().setStatusToApproved(); }
    void setCurrentLoanToRejected() { getCurrentLoan().setStatusToRejected(); }
    void setCurrentLoanPending() { getCurrentLoan().setStatusToPending(); }

    // Initialization function
    void setLoanIdArray(const BankDB& bankDb, int loanApplicationCount) {
        loanSet.clear();

        while (static_cast<int>(loanSet.size()) < loanApplicationCount) {
            int loanId = bankDb.getRandomLoanId();

            if (loanId == BankDB::invalidId) {
                break;
            }

            bool alreadyExists = false;

            for (int id : loanSet) {
                if (id == loanId) {
                    alreadyExists = true;
                    break;
                }
            }

            if (!alreadyExists) {
                loanSet.push_back(loanId);
            }
        }
    }

    LoanApplication& setLoanApplicationById(int loanId, const BankDB& bankDb) {
        loanApplication.setNewLoanApplication(loanId, bankDb);

        return loanApplication;
    }

    void initialize(const BankDB& bankDb, const Game& game) {
        setIndexToZero();
        setCurrentBudget(game.getStartingBudget());

        setLoanIdArray(bankDb, game.getLoanApplicationCount());

        if (!loanSet.empty()) {
            setLoanApplicationById(getCurrentLoanId(), bankDb);
        }
    }

    

    // Budget management
    void addToBudget(double amount) {
        if (amount > 0.0) {
            currentBudget += amount;
        }
    }

    bool canSubtractFromBudget(double amount) const {
        return amount >= 0.0 && amount <= currentBudget;
    }

    bool subtractFromBudget(double amount) {
        if (!canSubtractFromBudget(amount)) {
            return false;
        }

        currentBudget -= amount;
        return true;
    }

    // Loan proposal management
    void nextLoanApplication(const BankDB& bankDb) {
        if (loanSet.empty()) {
            return;
        }

        if (index + 1 >= static_cast<int>(loanSet.size())) {
            setIndex(static_cast<int>(loanSet.size()));
            return;
        }

        setIndex(index + 1);
        setLoanApplicationById(getCurrentLoanId(), bankDb);
    }

    void updateLoanStatusInput(BankDB& bankDb) {
        if (IsKeyPressed(KEY_ONE)) {
            approveLoan();
        }

        if (IsKeyPressed(KEY_TWO)) {
            rejectLoan();
        }

        if (IsKeyPressed(KEY_THREE)) {
            setLoanToPending();
        }

        if (IsKeyPressed(KEY_ENTER)) {
            nextLoan(bankDb);
        }
    }

    // Loan status
    bool canApproveLoan() const {
        if (getCurrentLoanId() == BankDB::invalidId) {
            return false;
        }

        return canSubtractFromBudget(getCurrentLoan().getRequestedAmount());
    }

    bool approveLoan() {
        if (!canApproveLoan()) {
            return false;
        }

        setCurrentLoanToApproved();
        return true;
    }

    bool rejectLoan() {
        if (getCurrentLoanId() == BankDB::invalidId) {
            return false;
        }

        setCurrentLoanToRejected();
        return true;
    }

    bool setLoanToPending() {
        if (getCurrentLoanId() == BankDB::invalidId) {
            return false;
        }

        setCurrentLoanPending();
        return true;
    }

    bool saveDecision(BankDB& bankDb) {
        int loanId = getCurrentLoanId();

        if (loanId == BankDB::invalidId) {
            return false;
        }

        Loan& loan = getCurrentLoan();

        if (loan.isPending()) {
            return false;
        }

        if (loan.isApproved()) {
            double requestedAmount = loan.getRequestedAmount();

            if (!canSubtractFromBudget(requestedAmount)) {
                return false;
            }

            if (!bankDb.approveLoanById(loanId)) {
                return false;
            }

            return subtractFromBudget(requestedAmount);
        }

        return bankDb.rejectLoanById(loanId);
    }

    // Turn management
    bool nextLoan(BankDB& bankDb) {
        if (getCurrentLoanId() == BankDB::invalidId) {
            return false;
        }

        if (getCurrentLoan().isPending()) {
            return false;
        }

        if (!saveDecision(bankDb)) {
            return false;
        }

        nextLoanApplication(bankDb);
        return true;
    }
    

    // LoanResult
    void setLoanResultById(int loanId, const BankDB& bankDb) {
        setLoanApplicationById(loanId, bankDb);

        Loan& loan = getCurrentLoan();

        if (loan.isApproved()) {
            loan.computeFinalResult();
            addToBudget(loan.getFinalAmount());
        }

        loanResults.push_back(loan);
    }

    void seeResult(Game& game, const BankDB& bankDb) {
        loanResults.clear();
        resultIndex = 0;

        for (int loanId : loanSet) {
            setLoanResultById(loanId, bankDb);
        }

        game.setGameResult(getCurrentBudget());
    }

    void showPreviousLoanResult() {
        if (loanResults.empty() || resultIndex <= 0) {
            return;
        }

        resultIndex--;
    }

    void showNextLoanResult() {
        if (loanResults.empty() || resultIndex + 1 >= static_cast<int>(loanResults.size())) {
            return;
        }

        resultIndex++;
    }
    
};