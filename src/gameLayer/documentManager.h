#pragma once

#include <raylib.h>
#include <algorithm>
#include <cstddef>
#include <vector>

#include "button.h"
#include "document.h"
#include "loanApplication.h"
#include "office.h"
#include "mouse.h"

class DocumentManager {
public:
    using DocumentType = Document::DocumentType;

private:
    std::vector<Document> documents{
        Document{ DocumentType::Client, { 280.0f, 140.0f } },
        Document{ DocumentType::FinancialSituation, { 310.0f, 90.0f } },
        Document{ DocumentType::CreditReport, { 310.0f, 90.0f } },
        Document{ DocumentType::EmploymentSituation, { 310.0f, 90.0f } },
        Document{ DocumentType::InternalBehavior, { 310.0f, 90.0f } },
        Document{ DocumentType::Loan, { 310.0f, 90.0f } }
    };

    std::vector<bool> documentVisibility;
    std::vector<DocumentType> documentOrder{ DocumentType::Client, DocumentType::FinancialSituation, DocumentType::CreditReport, DocumentType::EmploymentSituation, DocumentType::InternalBehavior, DocumentType::Loan };

    Document loanOutcomeDocument{ DocumentType::LoanOutcome, { 250.0f, 90.0f } };

    std::vector<Vector2> documentIconPositions;
    std::vector<Button> documentButtons;
    std::vector<Button> documentCloseButtons;

    Vector2 firstDocumentIconPosition{ 282.0f, 270.0f };
    Vector2 documentButtonOffset{ 32.0f, 32.0f };
    Vector2 documentButtonSize{ 32.0f, 32.0f };
    float documentIconHorizontalSpacing = 32.0f;

    Vector2 closeButtonSize{ 24.0f, 24.0f };
    float closeButtonMargin = 4.0f;

    DocumentType movingDocument = DocumentType::None;
    DocumentType closingDocument = DocumentType::None;
    Vector2 mouseOffset{};

    std::size_t getDocumentIndex(DocumentType documentType) const {
        for (std::size_t index = 0; index < documents.size(); index++) {
            if (documents[index].getType() == documentType) {
                return index;
            }
        }

        return documents.size();
    }

    bool isManagedDocument(DocumentType documentType) const { return getDocumentIndex(documentType) < documents.size(); }

    Document& getDocumentByType(DocumentType documentType) { return documents[getDocumentIndex(documentType)]; }
    const Document& getDocumentByType(DocumentType documentType) const { return documents[getDocumentIndex(documentType)]; }

    Rectangle getCloseButtonRectangle(const Document& document) const {
        Rectangle documentRectangle = document.getRectangle();
        return { documentRectangle.x + documentRectangle.width - closeButtonSize.x - closeButtonMargin, documentRectangle.y + closeButtonMargin, closeButtonSize.x, closeButtonSize.y };
    }

    void updateCloseButtonPosition(std::size_t documentIndex) {
        Rectangle closeButtonRectangle = getCloseButtonRectangle(documents[documentIndex]);
        documentCloseButtons[documentIndex].setRectangle(closeButtonRectangle);
    }

public:
    // Constructor
    DocumentManager() : documentVisibility(documents.size(), false) {
        documentIconPositions.reserve(documents.size());
        documentButtons.reserve(documents.size());
        documentCloseButtons.reserve(documents.size());

        for (std::size_t index = 0; index < documents.size(); index++) {
            Vector2 iconPosition{ firstDocumentIconPosition.x + documentIconHorizontalSpacing * static_cast<float>(index), firstDocumentIconPosition.y };
            Rectangle documentButtonRectangle{ iconPosition.x + documentButtonOffset.x, iconPosition.y + documentButtonOffset.y, documentButtonSize.x, documentButtonSize.y };
            Rectangle closeButtonRectangle = getCloseButtonRectangle(documents[index]);

            documentIconPositions.push_back(iconPosition);
            documentButtons.emplace_back("", documentButtonRectangle);
            documentCloseButtons.emplace_back("X", closeButtonRectangle);
            documentCloseButtons.back().setFontSize(14);
        }
    }

    // Getter functions
    DocumentType getMovingDocument() const { return movingDocument; }
    std::size_t getDocumentCount() const { return documents.size(); }

    bool isDocumentVisible(DocumentType documentType) const {
        std::size_t documentIndex = getDocumentIndex(documentType);

        if (documentIndex >= documents.size()) {
            return false;
        }

        return documentVisibility[documentIndex];
    }

    Rectangle getDocumentDestination(DocumentType documentType) const {
        if (!isManagedDocument(documentType)) {
            return {};
        }

        return getDocumentByType(documentType).getRectangle();
    }

    // Setter functions
    void setDocumentVisible(DocumentType documentType, bool visible) {
        std::size_t documentIndex = getDocumentIndex(documentType);

        if (documentIndex >= documents.size()) {
            return;
        }

        documentVisibility[documentIndex] = visible;
    }

    void setDocumentPosition(DocumentType documentType, Vector2 newPosition) {
        std::size_t documentIndex = getDocumentIndex(documentType);

        if (documentIndex >= documents.size()) {
            return;
        }

        documents[documentIndex].setPosition(newPosition);
        updateCloseButtonPosition(documentIndex);
    }

    // Visibility
    void toggleDocument(DocumentType documentType) {
        if (!isManagedDocument(documentType)) {
            return;
        }

        bool visible = !isDocumentVisible(documentType);
        setDocumentVisible(documentType, visible);

        if (visible) {
            bringDocumentToFront(documentType);
        }
    }

    void closeAllDocuments() {
        std::fill(documentVisibility.begin(), documentVisibility.end(), false);
        movingDocument = DocumentType::None;
        closingDocument = DocumentType::None;
    }

    void reset() { closeAllDocuments(); }

    // Document buttons
    void updateDocumentButtons(const Mouse& mouse) {
        if (movingDocument != DocumentType::None || closingDocument != DocumentType::None) {
            return;
        }

        for (std::size_t index = 0; index < documentButtons.size(); index++) {
            if (documentButtons[index].update(mouse)) {
                toggleDocument(documents[index].getType());
            }
        }
    }

    void updateDocumentCloseButtons(const Mouse& mouse) {
        if (mouse.isLeftPressed()) {
            closingDocument = DocumentType::None;

            for (auto iterator = documentOrder.rbegin(); iterator != documentOrder.rend(); iterator++) {
                DocumentType documentType = *iterator;

                if (!isDocumentVisible(documentType)) {
                    continue;
                }

                std::size_t documentIndex = getDocumentIndex(documentType);

                if (mouse.isInside(documentCloseButtons[documentIndex].getRectangle())) {
                    closingDocument = documentType;
                    break;
                }
            }
        }

        if (closingDocument == DocumentType::None) {
            return;
        }

        std::size_t documentIndex = getDocumentIndex(closingDocument);

        if (documentCloseButtons[documentIndex].update(mouse)) {
            setDocumentVisible(closingDocument, false);
        }

        if (mouse.isLeftReleased()) {
            closingDocument = DocumentType::None;
        }
    }

    // Drawing
    void drawDocumentIcons(const OfficeSprite& officeSprite) const {
        for (Vector2 iconPosition : documentIconPositions) {
            officeSprite.drawPaper(iconPosition);
        }
    }

    void drawDocuments(const LoanApplication& loanApplication) const {
        for (DocumentType documentType : documentOrder) {
            if (!isDocumentVisible(documentType)) {
                continue;
            }

            std::size_t documentIndex = getDocumentIndex(documentType);
            documents[documentIndex].draw(loanApplication);
            documentCloseButtons[documentIndex].draw();
        }
    }

    void drawLoanOutcome(const Loan& loan) const {
        loanOutcomeDocument.drawLoanResult(loan);
    }

    // Document order
    void bringDocumentToFront(DocumentType documentType) {
        if (!isManagedDocument(documentType)) {
            return;
        }

        for (auto iterator = documentOrder.begin(); iterator != documentOrder.end(); iterator++) {
            if (*iterator == documentType) {
                documentOrder.erase(iterator);
                break;
            }
        }

        documentOrder.push_back(documentType);
    }

    // Document movement
    void moveDocuments(const Mouse& mouse) {
        Vector2 mousePosition = mouse.getPosition();

        if (mouse.isLeftPressed()) {
            Rectangle selectedRectangle{};
            movingDocument = DocumentType::None;

            for (auto iterator = documentOrder.rbegin(); iterator != documentOrder.rend(); iterator++) {
                DocumentType documentType = *iterator;

                if (!isDocumentVisible(documentType)) {
                    continue;
                }

                Rectangle documentDestination = getDocumentDestination(documentType);

                if (mouse.isInside(documentDestination)) {
                    movingDocument = documentType;
                    selectedRectangle = documentDestination;
                    break;
                }
            }

            if (movingDocument != DocumentType::None) {
                mouseOffset = { mousePosition.x - selectedRectangle.x, mousePosition.y - selectedRectangle.y };
                bringDocumentToFront(movingDocument);
            }
        }

        if (mouse.isLeftReleased()) {
            movingDocument = DocumentType::None;
        }

        if (movingDocument != DocumentType::None) {
            Vector2 newPosition{ mousePosition.x - mouseOffset.x, mousePosition.y - mouseOffset.y };
            setDocumentPosition(movingDocument, newPosition);
        }
    }

    // Update
    void update(const Mouse& mouse) {
        updateDocumentCloseButtons(mouse);

        if (closingDocument == DocumentType::None) {
            moveDocuments(mouse);
            updateDocumentButtons(mouse);
        }
    }
};