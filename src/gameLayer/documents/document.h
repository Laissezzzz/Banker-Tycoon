#pragma once

#include <raylib.h>
#include <cstddef>
#include <sstream>
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
    struct Explanation {
        const char* title = "";
        const char* description = "";
    };

    DocumentType type = DocumentType::None;

    Vector2 position{ 0.0f, 0.0f };

    int fontSize = 8;
    int minimumFontSize = 5;
    int rows = 10;
    int columns = 2;

    float offset = 10.0f;
    float cellWidth = 140.0f;
    float cellHeight = 22.0f;
    float textPadding = 4.0f;

    float width = cellWidth * static_cast<float>(columns) + offset * 2.0f;
    float height = cellHeight * static_cast<float>(rows) + offset * 2.0f;

    int selectedExplanationRow = -1;

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

    int getFittingFontSize(const char* text, const Rectangle& cell) const {
        int fittingFontSize = fontSize;
        float availableWidth = cell.width - textPadding * 2.0f;

        while (fittingFontSize > minimumFontSize && static_cast<float>(MeasureText(text, fittingFontSize)) > availableWidth) {
            fittingFontSize--;
        }

        return fittingFontSize;
    }

    int getRowAtPosition(Vector2 pointerPosition) const {
        Rectangle grid = getGridRectangle();

        if (!CheckCollisionPointRec(pointerPosition, grid)) {
            return -1;
        }

        float currentCellHeight = grid.height / static_cast<float>(rows);
        int row = static_cast<int>((pointerPosition.y - grid.y) / currentCellHeight);
        return row >= 0 && row < rows ? row : -1;
    }

    int getLastExplanationRow() const {
        switch (type) {
        case DocumentType::EmploymentSituation:
            return 4;

        case DocumentType::Client:
        case DocumentType::FinancialSituation:
        case DocumentType::CreditReport:
        case DocumentType::InternalBehavior:
        case DocumentType::Loan:
        case DocumentType::LoanOutcome:
            return 5;

        case DocumentType::None:
        case DocumentType::GameSummary:
        case DocumentType::Count:
            return -1;
        }

        return -1;
    }

    Explanation getExplanation(int row) const {
        switch (type) {
        case DocumentType::Client:
            switch (row) {
            case 0: return { "Dati cliente", "Raccoglie le informazioni anagrafiche e familiari utili a conoscere il richiedente." };
            case 1: return { "Nome", "Identifica il cliente a cui appartiene la richiesta di prestito." };
            case 2: return { "Cognome", "Completa l'identificazione del cliente che richiede il finanziamento." };
            case 3: return { "Data di nascita", "Permette di ricavare l'età e di valutarla insieme alla durata del prestito." };
            case 4: return { "Stato civile", "Descrive la situazione familiare, da considerare insieme agli altri dati economici." };
            case 5: return { "Numero di figli", "Può indicare la presenza di spese familiari ricorrenti a carico del cliente." };
            }
            break;

        case DocumentType::FinancialSituation:
            switch (row) {
            case 0: return { "Situazione finanziaria", "Riassume liquidità, risparmi, patrimonio e garanzie disponibili del cliente." };
            case 1: return { "Saldo medio", "Importo mediamente presente sul conto nel periodo considerato; segnala la liquidità abituale." };
            case 2: return { "Risparmi disponibili", "Risorse accumulate che possono assorbire spese impreviste o difficoltà temporanee." };
            case 3: return { "Valore immobili", "Valore stimato degli immobili posseduti; è patrimonio, ma non liquidità immediata." };
            case 4: return { "Valore investimenti", "Valore delle attività finanziarie possedute, che può variare nel tempo." };
            case 5: return { "Garanzie disponibili", "Valore dei beni utilizzabili a tutela della banca; non elimina il rischio di insolvenza." };
            }
            break;

        case DocumentType::CreditReport:
            switch (row) {
            case 0: return { "Centrale rischi", "Riassume i debiti del cliente e le difficoltà di pagamento registrate nel sistema creditizio." };
            case 1: return { "Esposizione totale", "Somma complessiva dei debiti e dei finanziamenti ancora a carico del cliente." };
            case 2: return { "Prestiti attivi", "Numero di finanziamenti che il cliente sta già rimborsando." };
            case 3: return { "Rate mensili", "Totale delle rate pagate ogni mese; riduce il reddito disponibile per un nuovo prestito." };
            case 4: return { "Default pregressi", "Numero di finanziamenti passati che il cliente non è riuscito a rimborsare regolarmente." };
            case 5: return { "Inadempienze correnti", "Pagamenti attualmente scaduti o irregolari; sono un forte segnale di rischio." };
            }
            break;

        case DocumentType::EmploymentSituation:
            switch (row) {
            case 0: return { "Situazione lavorativa", "Descrive la fonte e la stabilità del reddito con cui il cliente dovrebbe pagare le rate." };
            case 1: return { "Professione", "Indica il lavoro svolto; va valutato senza separarlo da reddito e continuità lavorativa." };
            case 2: return { "Contratto", "Un contratto stabile rende normalmente più prevedibile il reddito futuro." };
            case 3: return { "Reddito mensile", "Entrata mensile utilizzabile per sostenere spese familiari, debiti e nuova rata." };
            case 4: return { "Anzianità lavorativa", "Tempo trascorso nello stesso impiego; può indicare continuità professionale." };
            }
            break;

        case DocumentType::InternalBehavior:
            switch (row) {
            case 0: return { "Andamento interno", "Mostra come il cliente ha gestito conto e prestiti nei rapporti precedenti con la banca." };
            case 1: return { "Anni da cliente", "Durata del rapporto con la banca e quantità di storia interna disponibile." };
            case 2: return { "Sconfinamenti", "Numero di volte in cui il conto ha superato la disponibilità o il limite concesso." };
            case 3: return { "Massimo sconfinamento", "Importo più elevato raggiunto oltre la disponibilità o il limite autorizzato." };
            case 4: return { "Pagamenti in ritardo", "Numero di pagamenti effettuati dopo la scadenza nei rapporti con la banca." };
            case 5: return { "Prestiti rimborsati", "Finanziamenti precedenti conclusi regolarmente dal cliente." };
            }
            break;

        case DocumentType::Loan:
            switch (row) {
            case 0: return { "Richiesta di prestito", "Raccoglie le condizioni del finanziamento che il cliente chiede alla banca." };
            case 1: return { "Finalità", "Indica lo scopo del finanziamento e aiuta a comprenderne necessità e caratteristiche." };
            case 2: return { "Importo richiesto", "Somma che la banca dovrebbe erogare se la domanda venisse approvata." };
            case 3: return { "Tasso di interesse", "Percentuale applicata al finanziamento che determina gli interessi dovuti dal cliente." };
            case 4: return { "Garanzie offerte", "Valore dei beni indicati a tutela della banca in caso di mancato rimborso." };
            case 5: return { "Stato", "Indica se la richiesta è ancora in attesa oppure è stata approvata o rifiutata." };
            }
            break;

        case DocumentType::LoanOutcome:
            switch (row) {
            case 0: return { "Risultato prestito", "Riassume la decisione presa e il risultato economico prodotto per la banca." };
            case 1: return { "Prestito ID", "Codice numerico che identifica in modo univoco la pratica." };
            case 2: return { "Importo richiesto", "Somma richiesta dal cliente nella pratica valutata." };
            case 3: return { "Decisione", "Scelta compiuta dal giocatore: approvare oppure rifiutare il prestito." };
            case 4: return { "Esito", "Indica se il prestito è stato rifiutato, rimborsato oppure è terminato in default." };
            case 5: return { "Risultato", "Guadagno o perdita generati dalla decisione per il bilancio della banca." };
            }
            break;

        case DocumentType::None:
        case DocumentType::GameSummary:
        case DocumentType::Count:
            break;
        }

        return {};
    }

    void drawWrappedText(const char* text, Rectangle area, int textFontSize, Color color) const {
        std::istringstream words(text);
        std::string word;
        std::string line;
        float linePositionY = area.y;
        float lineHeight = static_cast<float>(textFontSize + 2);

        while (words >> word) {
            std::string candidate = line.empty() ? word : line + " " + word;

            if (!line.empty() && static_cast<float>(MeasureText(candidate.c_str(), textFontSize)) > area.width) {
                if (linePositionY + static_cast<float>(textFontSize) > area.y + area.height) {
                    return;
                }

                DrawText(line.c_str(), static_cast<int>(area.x), static_cast<int>(linePositionY), textFontSize, color);
                linePositionY += lineHeight;
                line = word;
            }
            else {
                line = candidate;
            }
        }

        if (!line.empty() && linePositionY + static_cast<float>(textFontSize) <= area.y + area.height) {
            DrawText(line.c_str(), static_cast<int>(area.x), static_cast<int>(linePositionY), textFontSize, color);
        }
    }

    void drawExplanation() const {
        if (selectedExplanationRow < 0) {
            return;
        }

        Explanation explanation = getExplanation(selectedExplanationRow);

        if (explanation.description[0] == '\0') {
            return;
        }

        Rectangle grid = getGridRectangle();
        Rectangle firstExplanationCell = getCellRectangle(6, 0);
        Rectangle selectedRowCell = getCellRectangle(selectedExplanationRow, 0);
        Rectangle selectedRowRectangle{ grid.x, selectedRowCell.y, grid.width, selectedRowCell.height };
        Rectangle explanationRectangle{ grid.x, firstExplanationCell.y, grid.width, grid.y + grid.height - firstExplanationCell.y };

        DrawRectangleRec(explanationRectangle, { 255, 248, 220, 255 });
        DrawRectangleLinesEx(explanationRectangle, 1.0f, DARKGRAY);
        DrawRectangleLinesEx(selectedRowRectangle, 2.0f, DARKBLUE);

        DrawText(explanation.title, static_cast<int>(explanationRectangle.x + textPadding), static_cast<int>(explanationRectangle.y + 4.0f), 8, DARKBLUE);

        Rectangle textRectangle{ explanationRectangle.x + textPadding, explanationRectangle.y + 16.0f, explanationRectangle.width - textPadding * 2.0f, explanationRectangle.height - 19.0f };
        drawWrappedText(explanation.description, textRectangle, 7, BLACK);
    }

    std::string getMaritalStatusText(const Client& client) const {
        const std::string& maritalStatus = client.getMaritalStatus();

        if (maritalStatus == "married" || maritalStatus == "Married") {
            return "Sposato/a";
        }

        if (maritalStatus == "single" || maritalStatus == "Single") {
            return "Celibe/Nubile";
        }

        if (maritalStatus == "separated" || maritalStatus == "Separated") {
            return "Separato/a";
        }

        if (maritalStatus == "divorced" || maritalStatus == "Divorced") {
            return "Divorziato/a";
        }

        if (maritalStatus == "widowed" || maritalStatus == "Widowed") {
            return "Vedovo/a";
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
            float firstDataRowY = grid.y + currentCellHeight;
            DrawLineV({ x, firstDataRowY }, { x, grid.y + grid.height }, GRAY);
        }
    }

    void drawCellText(const char* text, int row, int column, Color color = BLACK) const {
        Rectangle cell = getCellRectangle(row, column);
        int fittingFontSize = getFittingFontSize(text, cell);

        int textX = static_cast<int>(cell.x + textPadding);
        int textY = static_cast<int>(cell.y + (cell.height - static_cast<float>(fittingFontSize)) / 2.0f);

        BeginScissorMode(static_cast<int>(cell.x + 1.0f), static_cast<int>(cell.y + 1.0f), static_cast<int>(cell.width - 2.0f), static_cast<int>(cell.height - 2.0f));
        DrawText(text, textX, textY, fittingFontSize, color);
        EndScissorMode();
    }

    void drawTitleText(const char* text) const {
        Rectangle grid = getGridRectangle();
        Rectangle firstCell = getCellRectangle(0, 0);
        Rectangle titleRectangle{ grid.x, firstCell.y, grid.width, firstCell.height };
        int fittingFontSize = getFittingFontSize(text, titleRectangle);

        int textX = static_cast<int>(titleRectangle.x + textPadding);
        int textY = static_cast<int>(titleRectangle.y + (titleRectangle.height - static_cast<float>(fittingFontSize)) / 2.0f);

        BeginScissorMode(static_cast<int>(titleRectangle.x + 1.0f), static_cast<int>(titleRectangle.y + 1.0f), static_cast<int>(titleRectangle.width - 2.0f), static_cast<int>(titleRectangle.height - 2.0f));
        DrawText(text, textX, textY, fittingFontSize, BLACK);
        EndScissorMode();
    }

    void drawField(const char* label, const char* value, int row, Color valueColor = BLACK) const {
        drawCellText(label, row, 0);
        drawCellText(value, row, 1, valueColor);
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
    bool isExplanationVisible() const { return selectedExplanationRow >= 0; }

    // Setter functions
    void setType(DocumentType newType) { type = newType; }
    void setPosition(Vector2 newPosition) { position = newPosition; }
    void setSize(Vector2 newSize) { width = newSize.x; height = newSize.y; }
    void setFontSize(int newFontSize) { fontSize = newFontSize; }

    bool toggleExplanation(Vector2 pointerPosition) {
        int row = getRowAtPosition(pointerPosition);

        if (row < 0 || row > getLastExplanationRow()) {
            selectedExplanationRow = -1;
            return false;
        }

        selectedExplanationRow = selectedExplanationRow == row ? -1 : row;
        return true;
    }

    void clearExplanation() { selectedExplanationRow = -1; }

    // Drawing individual documents
    void drawClientDocument(const Client& client) const {
        std::string maritalStatus = getMaritalStatusText(client);

        drawDocumentGrid();
        drawTitleText("DATI CLIENTE");
        drawField("Nome", client.getName().c_str(), 1);
        drawField("Cognome", client.getSurname().c_str(), 2);
        drawField("Data di nascita", client.getBirthDate().c_str(), 3);
        drawField("Stato civile", maritalStatus.c_str(), 4);
        drawField("Numero di figli", TextFormat("%d", client.getChildrenCount()), 5);
    }

    void drawFinancialSituation(const FinancialSituation& financialSituation) const {
        drawDocumentGrid();
        drawTitleText("SITUAZIONE FINANZIARIA");
        drawField("Saldo medio", TextFormat("%.2f$", financialSituation.getAverageBalance()), 1);
        drawField("Risparmi disponibili", TextFormat("%.2f$", financialSituation.getAvailableSavings()), 2);
        drawField("Valore immobili", TextFormat("%.2f$", financialSituation.getRealEstateValue()), 3);
        drawField("Valore investimenti", TextFormat("%.2f$", financialSituation.getInvestmentsValue()), 4);
        drawField("Garanzie disponibili", TextFormat("%.2f$", financialSituation.getAvailableGuarantees()), 5);
    }

    void drawCreditReport(const CreditReport& creditReport) const {
        drawDocumentGrid();
        drawTitleText("CENTRALE RISCHI");
        drawField("Esposizione totale", TextFormat("%.2f$", creditReport.getTotalExposure()), 1);
        drawField("Prestiti attivi", TextFormat("%d", creditReport.getActiveLoansCount()), 2);
        drawField("Rate mensili", TextFormat("%.2f$", creditReport.getTotalMonthlyPayments()), 3);
        drawField("Default pregressi", TextFormat("%d", creditReport.getPastDefaults()), 4);
        drawField("Inadempienze correnti", TextFormat("%d", creditReport.getCurrentDelinquencies()), 5);
    }

    void drawEmploymentSituation(const EmploymentSituation& employmentSituation) const {
        drawDocumentGrid();
        drawTitleText("SITUAZIONE LAVORATIVA");
        drawField("Professione", employmentSituation.getProfession().c_str(), 1);
        drawField("Contratto", EmploymentSituation::employmentTypeToString(employmentSituation.getEmploymentType()), 2);
        drawField("Reddito mensile", TextFormat("%.2f$", employmentSituation.getMonthlyIncome()), 3);
        drawField("Anzianità lavorativa", TextFormat("%.2f anni", employmentSituation.getYearsAtJob()), 4);
    }

    void drawInternalBehavior(const InternalBehavior& internalBehavior) const {
        drawDocumentGrid();
        drawTitleText("ANDAMENTO INTERNO");
        drawField("Anni da cliente", TextFormat("%.2f", internalBehavior.getYearsWithBank()), 1);
        drawField("Sconfinamenti", TextFormat("%d", internalBehavior.getOverdraftCount()), 2);
        drawField("Massimo sconfinamento", TextFormat("%.2f$", internalBehavior.getMaximumOverdraft()), 3);
        drawField("Pagamenti in ritardo", TextFormat("%d", internalBehavior.getLatePaymentCount()), 4);
        drawField("Prestiti rimborsati", TextFormat("%d", internalBehavior.getRegularlyRepaidLoans()), 5);
    }

    void drawLoan(const Loan& loan) const {
        drawDocumentGrid();
        drawTitleText("RICHIESTA DI PRESTITO");
        drawField("Finalità", loan.getPurpose().c_str(), 1);
        drawField("Importo richiesto", TextFormat("%.2f$", loan.getRequestedAmount()), 2);
        drawField("Tasso di interesse", TextFormat("%.2f%%", loan.getInterestRate() * 100.0), 3);
        drawField("Garanzie offerte", TextFormat("%.2f$", loan.getGuaranteesOffered()), 4);
        drawField("Stato", getLoanStatusText(loan), 5);
    }

    void drawLoanResult(const Loan& loan) const {
        drawDocumentGrid();
        drawTitleText("RISULTATO PRESTITO");
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

        drawExplanation();
    }
};
