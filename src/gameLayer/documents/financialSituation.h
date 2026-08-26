#pragma once

class FinancialSituation {
private:
    int financialSituationId = 0;
    int clientId = 0;
    double averageBalance = 0.0;
    double availableSavings = 0.0;
    double realEstateValue = 0.0;
    double investmentsValue = 0.0;
    double availableGuarantees = 0.0;

public:
    //Constructors
    FinancialSituation() = default;
    FinancialSituation(int fsid, int cId, double ab, double as, double rev, double iv, double ag)
        : financialSituationId(fsid), clientId(cId), averageBalance(ab), availableSavings(as), realEstateValue(rev), investmentsValue(iv), availableGuarantees(ag)
    {}

    //Getter functions
    int getFinancialSituationId() const {
        return financialSituationId;
    }

    int getClientId() const {
        return clientId;
    }

    double getAverageBalance() const {
        return averageBalance;
    }

    double getAvailableSavings() const {
        return availableSavings;
    }

    double getRealEstateValue() const {
        return realEstateValue;
    }

    double getInvestmentsValue() const {
        return investmentsValue;
    }

    double getAvailableGuarantees() const {
        return availableGuarantees;
    }
};