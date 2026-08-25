#pragma once

#include <raylib.h>

#include "bankDb.h"
#include "button.h"
#include "documentManager.h"
#include "game.h"
#include "hud.h"
#include "loanApplicationManager.h"
#include "mouse.h"
#include "office.h"

class LoanExamination {
private:
    OfficeSprite officeSprite;
    DocumentManager documentManager;
    Hud hud;

    Button acceptButton{ "Accept", { 570.0f, 335.0f, 95.0f, 45.0f } };
    Button rejectButton{ "Reject", { 675.0f, 335.0f, 95.0f, 45.0f } };
    Button nextLoanButton{ "Next loan", { 570.0f, 390.0f, 200.0f, 45.0f } };

    bool showInsufficientFundsMessage = false;
    bool examinationCompleted = false;

public:
    // Getter functions
    bool isFinished(const Game& game) const { return examinationCompleted || game.hasTimeExpired(); }
    DocumentManager& getDocumentManager() { return documentManager; }
    const DocumentManager& getDocumentManager() const { return documentManager; }

    // Setter functions
    void setCompleted(bool completed) { examinationCompleted = completed; }

    // Initialization
    void initialize() { officeSprite.load(); }

    void reset() {
        documentManager.reset();
        showInsufficientFundsMessage = false;
        setCompleted(false);
    }

    // Update
    void update(const Mouse& mouse, LoanApplicationManager& loanApplicationManager, BankDB& bankDb) {
        bool loanAvailable = loanApplicationManager.getCurrentLoanId() != BankDB::invalidId;

        if (!loanAvailable) {
            setCompleted(true);
            return;
        }

        Loan& currentLoan = loanApplicationManager.getCurrentLoan();
        bool decisionMade = !currentLoan.isPending();

        bool acceptClicked = acceptButton.update(mouse);
        bool rejectClicked = rejectButton.update(mouse);
        bool nextLoanClicked = false;

        if (decisionMade) {
            nextLoanClicked = nextLoanButton.update(mouse);
        }

        bool pointerInsideActionButton = acceptButton.isPointerInside() || rejectButton.isPointerInside();

        if (decisionMade) {
            pointerInsideActionButton = pointerInsideActionButton || nextLoanButton.isPointerInside();
        }

        if (!pointerInsideActionButton || documentManager.getMovingDocument() != Document::DocumentType::None) {
            documentManager.update(mouse);
        }

        if (acceptClicked) {
            if (loanApplicationManager.approveLoan()) {
                showInsufficientFundsMessage = false;
            }
            else {
                showInsufficientFundsMessage = true;
            }
        }

        if (rejectClicked) {
            loanApplicationManager.rejectLoan();
            showInsufficientFundsMessage = false;
        }

        if (nextLoanClicked) {
            if (loanApplicationManager.nextLoan(bankDb)) {
                documentManager.reset();
                showInsufficientFundsMessage = false;

                if (loanApplicationManager.getCurrentLoanId() == BankDB::invalidId) {
                    setCompleted(true);
                }
            }
        }

        if (currentLoan.isApproved()) {
            acceptButton.setNormalColor(GREEN);
            rejectButton.setNormalColor(LIGHTGRAY);
        }
        else if (currentLoan.isRejected()) {
            acceptButton.setNormalColor(LIGHTGRAY);
            rejectButton.setNormalColor(GREEN);
        }
        else {
            acceptButton.setNormalColor(LIGHTGRAY);
            rejectButton.setNormalColor(LIGHTGRAY);
        }
    }

    // Drawing
    void draw(LoanApplicationManager& loanApplicationManager, const Game& game) {
        officeSprite.drawOffice();
        documentManager.drawDocumentIcons(officeSprite);
        hud.drawHud(loanApplicationManager, game, game.getPlayer());
        documentManager.drawDocuments(loanApplicationManager.getLoanApplication());
        

        if (showInsufficientFundsMessage) {
            DrawText("You don't have enough funds!", 570, 320, 10, RED);
        }

        bool loanAvailable = loanApplicationManager.getCurrentLoanId() != BankDB::invalidId;

        if (loanAvailable) {
            acceptButton.draw();
            rejectButton.draw();

            if (!loanApplicationManager.getCurrentLoan().isPending()) {
                nextLoanButton.draw();
            }
        }
    }
};