#pragma once

#include "bankDb.h"
#include "client.h"
#include "creditReport.h"
#include "employmentSituation.h"
#include "financialSituation.h"
#include "internalBehavior.h"
#include "loan.h"

class LoanApplication {
private:
    Loan loan;
    Client client;
    CreditReport creditReport;
    EmploymentSituation employmentSituation;
    FinancialSituation financialSituation;
    InternalBehavior internalBehavior;

public:
    LoanApplication() = default;

    LoanApplication(int loanId, const BankDB& bankDB) :
        loan(bankDB.getLoanFromId(loanId)),
        client(bankDB.getClientFromId(loan.getClientId())),
        creditReport(bankDB.getCreditReportFromClientId(loan.getClientId())),
        employmentSituation(bankDB.getEmploymentSituationFromClientId(loan.getClientId())),
        financialSituation(bankDB.getFinancialSituationFromClientId(loan.getClientId())),
        internalBehavior(bankDB.getInternalBehaviorFromClientId(loan.getClientId())) {}

    // Getter functions
    Loan& getLoan() { return loan; }
    Client& getClient() { return client; }
    CreditReport& getCreditReport() { return creditReport; }
    EmploymentSituation& getEmploymentSituation() { return employmentSituation; }
    FinancialSituation& getFinancialSituation() { return financialSituation; }
    InternalBehavior& getInternalBehavior() { return internalBehavior; }

    // Setter functions
    const Loan& getLoan() const { return loan; }
    const Client& getClient() const { return client; }
    const CreditReport& getCreditReport() const { return creditReport; }
    const EmploymentSituation& getEmploymentSituation() const { return employmentSituation; }
    const FinancialSituation& getFinancialSituation() const { return financialSituation; }
    const InternalBehavior& getInternalBehavior() const { return internalBehavior; }

    // Set a new loan application
    void setNewLoanApplication(int loanId, const BankDB& bankDB) {
        loan = bankDB.getLoanFromId(loanId);

        int clientId = loan.getClientId();

        client = bankDB.getClientFromId(clientId);
        creditReport = bankDB.getCreditReportFromClientId(clientId);
        employmentSituation = bankDB.getEmploymentSituationFromClientId(clientId);
        financialSituation = bankDB.getFinancialSituationFromClientId(clientId);
        internalBehavior = bankDB.getInternalBehaviorFromClientId(clientId);
    }
};