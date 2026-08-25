#pragma once

class InternalBehavior {
private:
    int internalBehaviorId = 0;
    int clientId = 0;
    double yearsWithBank = 0.0;
    int overdraftCount = 0;
    double maximumOverdraft = 0.0;
    int latePaymentCount = 0;
    int regularlyRepaidLoans = 0;

public:
    InternalBehavior() = default;

    InternalBehavior(int ibid, int cId, double ywb, int oc, double mo, int lpc, int rrl)
        : internalBehaviorId(ibid), clientId(cId), yearsWithBank(ywb), overdraftCount(oc), maximumOverdraft(mo), latePaymentCount(lpc), regularlyRepaidLoans(rrl)
    {}

    int getInternalBehaviorId() const {
        return internalBehaviorId;
    }

    int getClientId() const {
        return clientId;
    }

    double getYearsWithBank() const {
        return yearsWithBank;
    }

    int getOverdraftCount() const {
        return overdraftCount;
    }

    double getMaximumOverdraft() const {
        return maximumOverdraft;
    }

    int getLatePaymentCount() const {
        return latePaymentCount;
    }

    int getRegularlyRepaidLoans() const {
        return regularlyRepaidLoans;
    }
};