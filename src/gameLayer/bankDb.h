#pragma once

#include "database.h"
#include "client.h"
#include "creditReport.h"
#include "employmentSituation.h"
#include "financialSituation.h"
#include "internalBehavior.h"
#include "loan.h"

#include <iostream>
#include <string>

class BankDB {
public:
    static constexpr int invalidId = -1;

private:
    Database database{};
    std::string pathAndName = "bankDB/bank.db";
    static std::string getText(sqlite3_stmt* statement, int column) {
        const unsigned char* text = sqlite3_column_text(statement, column);
        return text != nullptr ? reinterpret_cast<const char*>(text) : "";
    }

public:

    // Database operation functions
    bool initialize() {
        const std::string path = std::string(RESOURCES_PATH) + pathAndName;
        std::cout << "Apro database: " << path << '\n';

        if (!database.open(path)) {
            std::cerr << "Impossibile inizializzare bank.db\n";
            return false;
        }

        return true;
    }

    void close() {
        database.close();
    }

    bool isOpen() const {
        return database.isOpen();
    }

    bool resetLoanStatuses() {
        if (!database.isOpen()) {
            return false;
        }

        const char* query = "UPDATE loans SET status = 'pending', decision_reason = '';";

        if (!database.execute(query)) {
            return false;
        }

        return true;
    }

    // Getter functions
    int getClientSize() const {
        if (!database.isOpen()) {
            std::cerr << "BankDB non inizializzato\n";
            return 0;
        }

        const char* query = "SELECT COUNT(*) FROM clients;";
        sqlite3_stmt* statement = nullptr;

        if (sqlite3_prepare_v2(database.getHandle(), query, -1, &statement, nullptr) != SQLITE_OK) {
            std::cerr << "Errore preparazione query: " << sqlite3_errmsg(database.getHandle()) << '\n';
            return 0;
        }

        int clientSize = 0;

        if (sqlite3_step(statement) == SQLITE_ROW) {
            clientSize = sqlite3_column_int(statement, 0);
        }
        else {
            std::cerr << "Errore conteggio clienti: " << sqlite3_errmsg(database.getHandle()) << '\n';
        }

        sqlite3_finalize(statement);
        return clientSize;
    }

    Client getClientFromId(int clientId) const {
        if (!database.isOpen()) {
            std::cerr << "BankDB non inizializzato\n";
            return Client{};
        }

        const char* query = "SELECT id_client, name, surname, birth_date, marital_status, children_count FROM clients WHERE id_client = ?;";
        sqlite3_stmt* statement = nullptr;

        if (sqlite3_prepare_v2(database.getHandle(), query, -1, &statement, nullptr) != SQLITE_OK) {
            std::cerr << "Errore preparazione query: " << sqlite3_errmsg(database.getHandle()) << '\n';
            return Client{};
        }

        if (sqlite3_bind_int(statement, 1, clientId) != SQLITE_OK) {
            std::cerr << "Errore inserimento ID: " << sqlite3_errmsg(database.getHandle()) << '\n';
            sqlite3_finalize(statement);
            return Client{};
        }

        const int result = sqlite3_step(statement);

        if (result == SQLITE_DONE) {
            std::cerr << "Cliente con ID " << clientId << " non trovato\n";
            sqlite3_finalize(statement);
            return Client{};
        }

        if (result != SQLITE_ROW) {
            std::cerr << "Errore lettura cliente: " << sqlite3_errmsg(database.getHandle()) << '\n';
            sqlite3_finalize(statement);
            return Client{};
        }

        Client client{sqlite3_column_int(statement, 0),getText(statement, 1),getText(statement, 2),
            getText(statement, 3),getText(statement, 4),sqlite3_column_int(statement, 5)};

        sqlite3_finalize(statement);
        return client;
    }

    int getRandomClientId() const {
        if (!database.isOpen()) {
            std::cerr << "BankDB non inizializzato\n";
            return invalidId;
        }

        const char* query = "SELECT id_client FROM clients ORDER BY RANDOM() LIMIT 1;";
        sqlite3_stmt* statement = nullptr;

        if (sqlite3_prepare_v2(database.getHandle(), query, -1, &statement, nullptr) != SQLITE_OK) {
            std::cerr << "Errore preparazione query: " << sqlite3_errmsg(database.getHandle()) << '\n';
            return invalidId;
        }

        int clientId = invalidId;

        if (sqlite3_step(statement) == SQLITE_ROW) {
            clientId = sqlite3_column_int(statement, 0);
        }
        else {
            std::cerr << "Nessun cliente disponibile\n";
        }

        sqlite3_finalize(statement);
        return clientId;
    }

    Client getRandomClient() const {
        const int clientId = getRandomClientId();

        if (clientId == invalidId) {
            return Client{};
        }

        return getClientFromId(clientId);
    }

    FinancialSituation getFinancialSituationFromClientId(int clientId) const {
        if (!database.isOpen()) {
            std::cerr << "BankDB non inizializzato\n";
            return FinancialSituation{};
        }

        const char* query = "SELECT id_financial, client_id, average_balance, available_savings, real_estate_value, investments_value, available_guarantees FROM financial_data WHERE client_id = ?;";
        sqlite3_stmt* statement = nullptr;

        if (sqlite3_prepare_v2(database.getHandle(), query, -1, &statement, nullptr) != SQLITE_OK) {
            std::cerr << "Errore preparazione query: " << sqlite3_errmsg(database.getHandle()) << '\n';
            return FinancialSituation{};
        }

        if (sqlite3_bind_int(statement, 1, clientId) != SQLITE_OK) {
            std::cerr << "Errore inserimento ID: " << sqlite3_errmsg(database.getHandle()) << '\n';
            sqlite3_finalize(statement);
            return FinancialSituation{};
        }

        const int result = sqlite3_step(statement);

        if (result == SQLITE_DONE) {
            std::cerr << "Situazione finanziaria del cliente con ID " << clientId << " non trovata\n";
            sqlite3_finalize(statement);
            return FinancialSituation{};
        }

        if (result != SQLITE_ROW) {
            std::cerr << "Errore lettura situazione finanziaria: " << sqlite3_errmsg(database.getHandle()) << '\n';
            sqlite3_finalize(statement);
            return FinancialSituation{};
        }

        FinancialSituation financialSituation{ sqlite3_column_int(statement, 0), sqlite3_column_int(statement, 1), sqlite3_column_double(statement, 2),
            sqlite3_column_double(statement, 3), sqlite3_column_double(statement, 4), sqlite3_column_double(statement, 5), sqlite3_column_double(statement, 6)
        };

        sqlite3_finalize(statement);
        return financialSituation;
    }

    FinancialSituation getFinancialSituationFromClient(const Client& client) const {
        return getFinancialSituationFromClientId(client.getId());
    }

    CreditReport getCreditReportFromClientId(int clientId) const {
        if (!database.isOpen()) {
            std::cerr << "BankDB non inizializzato\n";
            return CreditReport{};
        }

        const char* query = "SELECT id_credit_report, client_id, total_exposure, active_loans_count, total_monthly_payments, past_defaults, current_delinquencies, unlikely_to_pay_count FROM credit_reports WHERE client_id = ?;";
        sqlite3_stmt* statement = nullptr;

        if (sqlite3_prepare_v2(database.getHandle(), query, -1, &statement, nullptr) != SQLITE_OK) {
            std::cerr << "Errore preparazione query: " << sqlite3_errmsg(database.getHandle()) << '\n';
            return CreditReport{};
        }

        if (sqlite3_bind_int(statement, 1, clientId) != SQLITE_OK) {
            std::cerr << "Errore inserimento ID: " << sqlite3_errmsg(database.getHandle()) << '\n';
            sqlite3_finalize(statement);
            return CreditReport{};
        }

        const int result = sqlite3_step(statement);

        if (result == SQLITE_DONE) {
            std::cerr << "Credit report del cliente con ID " << clientId << " non trovato\n";
            sqlite3_finalize(statement);
            return CreditReport{};
        }

        if (result != SQLITE_ROW) {
            std::cerr << "Errore lettura credit report: " << sqlite3_errmsg(database.getHandle()) << '\n';
            sqlite3_finalize(statement);
            return CreditReport{};
        }

        CreditReport creditReport{ sqlite3_column_int(statement, 0), sqlite3_column_int(statement, 1), sqlite3_column_double(statement, 2),
            sqlite3_column_int(statement, 3), sqlite3_column_double(statement, 4), sqlite3_column_int(statement, 5), sqlite3_column_int(statement, 6),
            sqlite3_column_int(statement, 7)
        };

        sqlite3_finalize(statement);
        return creditReport;
    }

    CreditReport getCreditReportFromClient(const Client& client) const {
        return getCreditReportFromClientId(client.getId());
    }

    EmploymentSituation getEmploymentSituationFromClientId(int clientId) const {
        if (!database.isOpen()) {
            std::cerr << "BankDB non inizializzato\n";
            return EmploymentSituation{};
        }

        const char* query = "SELECT id_employment, client_id, profession, employment_type, monthly_income, years_at_job FROM employment_data WHERE client_id = ?;";
        sqlite3_stmt* statement = nullptr;

        if (sqlite3_prepare_v2(database.getHandle(), query, -1, &statement, nullptr) != SQLITE_OK) {
            std::cerr << "Errore preparazione query: " << sqlite3_errmsg(database.getHandle()) << '\n';
            return EmploymentSituation{};
        }

        if (sqlite3_bind_int(statement, 1, clientId) != SQLITE_OK) {
            std::cerr << "Errore inserimento ID: " << sqlite3_errmsg(database.getHandle()) << '\n';
            sqlite3_finalize(statement);
            return EmploymentSituation{};
        }

        const int result = sqlite3_step(statement);

        if (result == SQLITE_DONE) {
            std::cerr << "Situazione lavorativa del cliente con ID " << clientId << " non trovata\n";
            sqlite3_finalize(statement);
            return EmploymentSituation{};
        }

        if (result != SQLITE_ROW) {
            std::cerr << "Errore lettura situazione lavorativa: " << sqlite3_errmsg(database.getHandle()) << '\n';
            sqlite3_finalize(statement);
            return EmploymentSituation{};
        }

        EmploymentSituation employmentSituation{ sqlite3_column_int(statement, 0), sqlite3_column_int(statement, 1), getText(statement, 2),
            EmploymentSituation::stringToEmploymentType(getText(statement, 3)), sqlite3_column_double(statement, 4), sqlite3_column_double(statement, 5)
        };

        sqlite3_finalize(statement);
        return employmentSituation;
    }

    EmploymentSituation getEmploymentSituationFromClient(const Client& client) const {
        return getEmploymentSituationFromClientId(client.getId());
    }

    InternalBehavior getInternalBehaviorFromClientId(int clientId) const {
        if (!database.isOpen()) {
            std::cerr << "BankDB non inizializzato\n";
            return InternalBehavior{};
        }

        const char* query = "SELECT id_internal_behavior, client_id, years_with_bank, overdraft_count, maximum_overdraft, late_payment_count, regularly_repaid_loans FROM internal_behavior WHERE client_id = ?;";
        sqlite3_stmt* statement = nullptr;

        if (sqlite3_prepare_v2(database.getHandle(), query, -1, &statement, nullptr) != SQLITE_OK) {
            std::cerr << "Errore preparazione query: " << sqlite3_errmsg(database.getHandle()) << '\n';
            return InternalBehavior{};
        }

        if (sqlite3_bind_int(statement, 1, clientId) != SQLITE_OK) {
            std::cerr << "Errore inserimento ID: " << sqlite3_errmsg(database.getHandle()) << '\n';
            sqlite3_finalize(statement);
            return InternalBehavior{};
        }

        const int result = sqlite3_step(statement);

        if (result == SQLITE_DONE) {
            std::cerr << "Comportamento interno del cliente con ID " << clientId << " non trovato\n";
            sqlite3_finalize(statement);
            return InternalBehavior{};
        }

        if (result != SQLITE_ROW) {
            std::cerr << "Errore lettura comportamento interno: " << sqlite3_errmsg(database.getHandle()) << '\n';
            sqlite3_finalize(statement);
            return InternalBehavior{};
        }

        InternalBehavior internalBehavior{ sqlite3_column_int(statement, 0), sqlite3_column_int(statement, 1), sqlite3_column_double(statement, 2),
            sqlite3_column_int(statement, 3), sqlite3_column_double(statement, 4), sqlite3_column_int(statement, 5), sqlite3_column_int(statement, 6)
        };

        sqlite3_finalize(statement);
        return internalBehavior;
    }

    Loan getLoanFromId(int loanId) const {
        if (!database.isOpen()) {
            std::cerr << "BankDB non inizializzato\n";
            return Loan{};
        }

        const char* query = "SELECT id_loan, client_id, purpose, requested_amount, duration_months, interest_rate, guarantees_offered, status, default_probability, recovery_rate, real_risk, internal_rating, remaining_amount, decision_reason FROM loans WHERE id_loan = ?;";
        sqlite3_stmt* statement = nullptr;

        if (sqlite3_prepare_v2(database.getHandle(), query, -1, &statement, nullptr) != SQLITE_OK) {
            std::cerr << "Errore preparazione query: " << sqlite3_errmsg(database.getHandle()) << '\n';
            return Loan{};
        }

        if (sqlite3_bind_int(statement, 1, loanId) != SQLITE_OK) {
            std::cerr << "Errore inserimento ID: " << sqlite3_errmsg(database.getHandle()) << '\n';
            sqlite3_finalize(statement);
            return Loan{};
        }

        const int result = sqlite3_step(statement);

        if (result == SQLITE_DONE) {
            std::cerr << "Prestito con ID " << loanId << " non trovato\n";
            sqlite3_finalize(statement);
            return Loan{};
        }

        if (result != SQLITE_ROW) {
            std::cerr << "Errore lettura prestito: " << sqlite3_errmsg(database.getHandle()) << '\n';
            sqlite3_finalize(statement);
            return Loan{};
        }

        Loan loan{
            sqlite3_column_int(statement, 0),
            sqlite3_column_int(statement, 1),
            getText(statement, 2),
            sqlite3_column_double(statement, 3),
            sqlite3_column_int(statement, 4),
            sqlite3_column_double(statement, 5),
            sqlite3_column_double(statement, 6),
            Loan::stringToLoanStatus(getText(statement, 7)),
            sqlite3_column_double(statement, 8),
            sqlite3_column_double(statement, 9),
            Loan::stringToRiskLevel(getText(statement, 10)),
            getText(statement, 11),
            sqlite3_column_double(statement, 12),
            getText(statement, 13)
        };

        sqlite3_finalize(statement);
        return loan;
    }

    Loan getLoanFromClientId(int clientId) const {
        if (!database.isOpen()) {
            std::cerr << "BankDB non inizializzato\n";
            return Loan{};
        }

        const char* query = "SELECT id_loan, client_id, purpose, requested_amount, duration_months, interest_rate, guarantees_offered, status, default_probability, recovery_rate, real_risk, internal_rating, remaining_amount, decision_reason FROM loans WHERE client_id = ? ORDER BY id_loan LIMIT 1;";
        sqlite3_stmt* statement = nullptr;

        if (sqlite3_prepare_v2(database.getHandle(), query, -1, &statement, nullptr) != SQLITE_OK) {
            std::cerr << "Errore preparazione query: " << sqlite3_errmsg(database.getHandle()) << '\n';
            return Loan{};
        }

        if (sqlite3_bind_int(statement, 1, clientId) != SQLITE_OK) {
            std::cerr << "Errore inserimento ID: " << sqlite3_errmsg(database.getHandle()) << '\n';
            sqlite3_finalize(statement);
            return Loan{};
        }

        const int result = sqlite3_step(statement);

        if (result == SQLITE_DONE) {
            std::cerr << "Prestito del cliente con ID " << clientId << " non trovato\n";
            sqlite3_finalize(statement);
            return Loan{};
        }

        if (result != SQLITE_ROW) {
            std::cerr << "Errore lettura prestito: " << sqlite3_errmsg(database.getHandle()) << '\n';
            sqlite3_finalize(statement);
            return Loan{};
        }

        Loan loan{
            sqlite3_column_int(statement, 0),
            sqlite3_column_int(statement, 1),
            getText(statement, 2),
            sqlite3_column_double(statement, 3),
            sqlite3_column_int(statement, 4),
            sqlite3_column_double(statement, 5),
            sqlite3_column_double(statement, 6),
            Loan::stringToLoanStatus(getText(statement, 7)),
            sqlite3_column_double(statement, 8),
            sqlite3_column_double(statement, 9),
            Loan::stringToRiskLevel(getText(statement, 10)),
            getText(statement, 11),
            sqlite3_column_double(statement, 12),
            getText(statement, 13)
        };

        sqlite3_finalize(statement);
        return loan;
    }

    Loan getLoanFromClient(const Client& client) const {
        return getLoanFromClientId(client.getId());
    }

    int getRandomLoanId() const {
        if (!database.isOpen()) {
            std::cerr << "BankDB non inizializzato\n";
            return invalidId;
        }

        const char* query = "SELECT id_loan FROM loans ORDER BY RANDOM() LIMIT 1;";
        sqlite3_stmt* statement = nullptr;

        if (sqlite3_prepare_v2(database.getHandle(), query, -1, &statement, nullptr) != SQLITE_OK) {
            std::cerr << "Errore preparazione query: " << sqlite3_errmsg(database.getHandle()) << '\n';
            return invalidId;
        }

        int loanId = invalidId;

        if (sqlite3_step(statement) == SQLITE_ROW) {
            loanId = sqlite3_column_int(statement, 0);
        }
        else {
            std::cerr << "Nessun prestito disponibile\n";
        }

        sqlite3_finalize(statement);
        return loanId;
    }

    bool approveLoanById(int loanId) {
        if (!database.isOpen()) {
            std::cerr << "BankDB non inizializzato\n";
            return false;
        }

        const char* query = "UPDATE loans SET status = 'approved' WHERE id_loan = ?;";
        sqlite3_stmt* statement = nullptr;

        if (sqlite3_prepare_v2(database.getHandle(), query, -1, &statement, nullptr) != SQLITE_OK) {
            std::cerr << "Errore preparazione query: " << sqlite3_errmsg(database.getHandle()) << '\n';
            return false;
        }

        if (sqlite3_bind_int(statement, 1, loanId) != SQLITE_OK) {
            std::cerr << "Errore inserimento ID: " << sqlite3_errmsg(database.getHandle()) << '\n';
            sqlite3_finalize(statement);
            return false;
        }

        if (sqlite3_step(statement) != SQLITE_DONE) {
            std::cerr << "Errore approvazione prestito: " << sqlite3_errmsg(database.getHandle()) << '\n';
            sqlite3_finalize(statement);
            return false;
        }

        bool updated = sqlite3_changes(database.getHandle()) > 0;
        sqlite3_finalize(statement);

        return updated;
    }

    bool rejectLoanById(int loanId) {
        if (!database.isOpen()) {
            std::cerr << "BankDB non inizializzato\n";
            return false;
        }

        const char* query = "UPDATE loans SET status = 'rejected' WHERE id_loan = ?;";
        sqlite3_stmt* statement = nullptr;

        if (sqlite3_prepare_v2(database.getHandle(), query, -1, &statement, nullptr) != SQLITE_OK) {
            std::cerr << "Errore preparazione query: " << sqlite3_errmsg(database.getHandle()) << '\n';
            return false;
        }

        if (sqlite3_bind_int(statement, 1, loanId) != SQLITE_OK) {
            std::cerr << "Errore inserimento ID: " << sqlite3_errmsg(database.getHandle()) << '\n';
            sqlite3_finalize(statement);
            return false;
        }

        if (sqlite3_step(statement) != SQLITE_DONE) {
            std::cerr << "Errore rifiuto prestito: " << sqlite3_errmsg(database.getHandle()) << '\n';
            sqlite3_finalize(statement);
            return false;
        }

        bool updated = sqlite3_changes(database.getHandle()) > 0;
        sqlite3_finalize(statement);

        return updated;
    }

};