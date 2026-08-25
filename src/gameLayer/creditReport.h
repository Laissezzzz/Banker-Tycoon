#pragma once

class CreditReport {
private:
    int creditReportId              = 0;
    int clientId                    = 0;
    double totalExposure            = 0.0;
    int activeLoansCount            = 0;
    double totalMonthlyPayments     = 0.0;
    int pastDefaults                = 0;
    int currentDelinquencies        = 0;
    int unlikelyToPayCount          = 0;

public:
    CreditReport() = default;

    CreditReport(int crid, int cId, double te, int alc, double tmp, int pd, int cd, int utp)
        : creditReportId(crid), clientId(cId), totalExposure(te), activeLoansCount(alc), totalMonthlyPayments(tmp), pastDefaults(pd), currentDelinquencies(cd), unlikelyToPayCount(utp)
    {}

    int getCreditReportId() const {
        return creditReportId;
    }

    int getClientId() const {
        return clientId;
    }

    double getTotalExposure() const {
        return totalExposure;
    }

    int getActiveLoansCount() const {
        return activeLoansCount;
    }

    double getTotalMonthlyPayments() const {
        return totalMonthlyPayments;
    }

    int getPastDefaults() const {
        return pastDefaults;
    }

    int getCurrentDelinquencies() const {
        return currentDelinquencies;
    }

    int getUnlikelyToPayCount() const {
        return unlikelyToPayCount;
    }
};