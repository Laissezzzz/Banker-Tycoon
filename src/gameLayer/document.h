#pragma once

#include <raylib.h>
#include <cstddef>
#include <string>

#include "client.h"
#include "financialSituation.h"
#include "creditReport.h"
#include "employmentSituation.h"
#include "internalBehavior.h"
#include "loan.h"
#include "loanApplication.h"

class Document {
public:
    enum class DocumentType { None = -1, Client, FinancialSituation, CreditReport, EmploymentSituation, InternalBehavior, Loan, LoanOutcome, GameSummary, Count };

private:
    DocumentType type = DocumentType::None;

    Vector2 position{ 0.0f, 0.0f };

    int fontSize = 8;
    int rows = 10;
    int columns = 2;

    float offset = 10.0f;
    float cellWidth = 140.0f;
    float cellHeight = 22.0f;

    float width = cellWidth * static_cast<float>(columns) + offset * 2.0f;
    float height = cellHeight * static_cast<float>(rows) + offset * 2.0f;

    Rectangle getGridRectangle() const {
        Rectangle documentRectangle = getRectangle();
        return { documentRectangle.x + offset, documentRectangle.y + offset, documentRectangle.width - offset * 2.0f, documentRectangle.height - offset * 2.0f };
    }

    Rectangle getCellRectangle(int row, int column) const {
        Rectangle grid = getGridRectangle();
        float currentCellWidth = grid.width / static_cast<float>(columns);
        float currentCellHeight = grid.height / static_cast<float>(rows);

        return { grid.x + currentCellWidth * static_cast<float>(column), grid.y + currentCellHeight * static_cast<float>(row), currentCellWidth, currentCellHeight };
    }

    std::string getMaritalStatusText(const Client& client) const {
        const std::string& maritalStatus = client.getMaritalStatus();

        if (maritalStatus == "married" || maritalStatus == "Married") {
            return "Sposato/a";
        }

        if (maritalStatus == "single" || maritalStatus == "Single") {
            return "Celibe/Nubile";
        }

        return maritalStatus;
    }

    const char* getLoanStatusText(const Loan& loan) const {
        if (loan.isApproved()) {
            return "Approvato";
        }

        if (loan.isRejected()) {
            return "Rifiutato";
        }

        return "In attesa";
    }

    void drawDocumentGrid() const {
        Rectangle documentRectangle = getRectangle();
        Rectangle grid = getGridRectangle();
        float currentCellWidth = grid.width / static_cast<float>(columns);
        float currentCellHeight = grid.height / static_cast<float>(rows);

        DrawRectangleRec(documentRectangle, RAYWHITE);
        DrawRectangleLinesEx(documentRectangle, 2.0f, BLACK);
        DrawRectangleLinesEx(grid, 1.0f, GRAY);

        for (int row = 1; row < rows; row++) {
            float y = grid.y + currentCellHeight * static_cast<float>(row);
            DrawLineV({ grid.x, y }, { grid.x + grid.width, y }, GRAY);
        }

        for (int column = 1; column < columns; column++) {
            float x = grid.x + currentCellWidth * static_cast<float>(column);
            DrawLineV({ x, grid.y }, { x, grid.y + grid.height }, GRAY);
        }
    }

    void drawCellText(const char* text, int row, int column) const {
        Rectangle cell = getCellRectangle(row, column);
        DrawText(text, static_cast<int>(cell.x + 4.0f), static_cast<int>(cell.y + (cell.height - static_cast<float>(fontSize)) / 2.0f), fontSize, BLACK);
    }

    void drawField(const char* label, const char* value, int row, Color valueColor = BLACK) const {
        drawCellText(label, row, 0);
        Rectangle cell = getCellRectangle(row, 1);
        DrawText(value, static_cast<int>(cell.x + 4.0f), static_cast<int>(cell.y + (cell.height - static_cast<float>(fontSize)) / 2.0f), fontSize, valueColor);
    }

public:
    // Constructors
    Document() = default;
    Document(DocumentType newType, Vector2 newPosition) : type(newType), position(newPosition) {}

    // Getter functions
    DocumentType getType() const { return type; }
    Vector2 getPosition() const { return position; }
    Vector2 getSize() const { return { width, height }; }
    float getWidth() const { return width; }
    float getHeight() const { return height; }
    int getFontSize() const { return fontSize; }
    Rectangle getRectangle() const { return { position.x, position.y, width, height }; }
    std::size_t getNumberOfDocuments() const { return static_cast<std::size_t>(DocumentType::Count); }

    // Setter functions
    void setType(DocumentType newType) { type = newType; }
    void setPosition(Vector2 newPosition) { position = newPosition; }
    void setSize(Vector2 newSize) { width = newSize.x; height = newSize.y; }
    void setFontSize(int newFontSize) { fontSize = newFontSize; }

    // Drawing individual documents
    void drawClientDocument(const Client& client) const {
        std::string maritalStatus = "Celibe/Nubile";

        if (client.getMaritalStatus() == "Married") {
            maritalStatus = "Sposato/a";
        }

        drawDocumentGrid();
        drawCellText("DATI CLIENTE", 0, 0);
        drawField("Nome", client.getName().c_str(), 1);
        drawField("Cognome", client.getSurname().c_str(), 2);
        drawField("Data di nascita", client.getBirthDate().c_str(), 3);
        drawField("Stato civile", maritalStatus.c_str(), 4);
        drawField("Numero di figli", TextFormat("%d", client.getChildrenCount()), 5);
    }

    void drawFinancialSituation(const FinancialSituation& financialSituation) const {
        drawDocumentGrid();
        drawCellText("SITUAZIONE FINANZIARIA", 0, 0);
        drawField("Saldo medio", TextFormat("%.2f$", financialSituation.getAverageBalance()), 1);
        drawField("Risparmi disponibili", TextFormat("%.2f$", financialSituation.getAvailableSavings()), 2);
        drawField("Valore immobili", TextFormat("%.2f$", financialSituation.getRealEstateValue()), 3);
        drawField("Valore investimenti", TextFormat("%.2f$", financialSituation.getInvestmentsValue()), 4);
        drawField("Garanzie disponibili", TextFormat("%.2f$", financialSituation.getAvailableGuarantees()), 5);
    }

    void drawCreditReport(const CreditReport& creditReport) const {
        drawDocumentGrid();
        drawCellText("CENTRALE RISCHI", 0, 0);
        drawField("Esposizione totale", TextFormat("%.2f$", creditReport.getTotalExposure()), 1);
        drawField("Prestiti attivi", TextFormat("%d", creditReport.getActiveLoansCount()), 2);
        drawField("Rate mensili", TextFormat("%.2f$", creditReport.getTotalMonthlyPayments()), 3);
        drawField("Default pregressi", TextFormat("%d", creditReport.getPastDefaults()), 4);
        drawField("Inadempienze correnti", TextFormat("%d", creditReport.getCurrentDelinquencies()), 5);
    }

    void drawEmploymentSituation(const EmploymentSituation& employmentSituation) const {
        drawDocumentGrid();
        drawCellText("SITUAZIONE LAVORATIVA", 0, 0);
        drawField("Professione", employmentSituation.getProfession().c_str(), 1);
        drawField("Contratto", EmploymentSituation::employmentTypeToString(employmentSituation.getEmploymentType()), 2);
        drawField("Reddito mensile", TextFormat("%.2f$", employmentSituation.getMonthlyIncome()), 3);
        drawField("Anzianità lavorativa", TextFormat("%.2f anni", employmentSituation.getYearsAtJob()), 4);
    }

    void drawInternalBehavior(const InternalBehavior& internalBehavior) const {
        drawDocumentGrid();
        drawCellText("ANDAMENTO INTERNO", 0, 0);
        drawField("Anni da cliente", TextFormat("%.2f", internalBehavior.getYearsWithBank()), 1);
        drawField("Sconfinamenti", TextFormat("%d", internalBehavior.getOverdraftCount()), 2);
        drawField("Massimo sconfinamento", TextFormat("%.2f$", internalBehavior.getMaximumOverdraft()), 3);
        drawField("Pagamenti in ritardo", TextFormat("%d", internalBehavior.getLatePaymentCount()), 4);
        drawField("Prestiti rimborsati", TextFormat("%d", internalBehavior.getRegularlyRepaidLoans()), 5);
    }

    void drawLoan(const Loan& loan) const {
        drawDocumentGrid();
        drawCellText("RICHIESTA DI PRESTITO", 0, 0);
        drawField("Finalità", loan.getPurpose().c_str(), 1);
        drawField("Importo richiesto", TextFormat("%.2f$", loan.getRequestedAmount()), 2);
        drawField("Tasso di interesse", TextFormat("%.2f%%", loan.getInterestRate() * 100.0), 3);
        drawField("Garanzie offerte", TextFormat("%.2f$", loan.getGuaranteesOffered()), 4);
        drawField("Stato", getLoanStatusText(loan), 5);
    }

    void drawLoanResult(const Loan& loan) const {
        drawDocumentGrid();
        drawCellText("RISULTATO PRESTITO", 0, 0);
        drawField("Prestito ID", TextFormat("%d", loan.getLoanId()), 1);
        drawField("Importo richiesto", TextFormat("%.2f$", loan.getRequestedAmount()), 2);
        drawField("Decisione", getLoanStatusText(loan), 3);

        if (loan.isRejected()) {
            drawField("Esito", "Prestito non concesso", 4);
        }
        else if (loan.hasDefaulted()) {
            drawField("Esito", "Default", 4);
        }
        else {
            drawField("Esito", "Rimborsato", 4);
        }

        double result = loan.computeProfitOrLoss();
        Color resultColor = BLACK;

        if (result > 0.0) {
            resultColor = DARKGREEN;
        }
        else if (result < 0.0) {
            resultColor = RED;
        }

        drawField("Risultato", TextFormat("%.2f$", result), 5, resultColor);
    }

    // Drawing selected document type
    void draw(const LoanApplication& loanApplication) const {
        switch (type) {
        case DocumentType::Client:
            drawClientDocument(loanApplication.getClient());
            break;

        case DocumentType::FinancialSituation:
            drawFinancialSituation(loanApplication.getFinancialSituation());
            break;

        case DocumentType::CreditReport:
            drawCreditReport(loanApplication.getCreditReport());
            break;

        case DocumentType::EmploymentSituation:
            drawEmploymentSituation(loanApplication.getEmploymentSituation());
            break;

        case DocumentType::InternalBehavior:
            drawInternalBehavior(loanApplication.getInternalBehavior());
            break;

        case DocumentType::Loan:
            drawLoan(loanApplication.getLoan());
            break;

        case DocumentType::LoanOutcome:
            drawLoanResult(loanApplication.getLoan());
            break;

        case DocumentType::None:
        case DocumentType::GameSummary:
        case DocumentType::Count:
            break;
        }
    }
};