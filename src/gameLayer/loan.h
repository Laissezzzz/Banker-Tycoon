#pragma once

#include <raylib.h>
#include <string>

class Loan {
public:
    enum class LoanStatus { Pending, Approved, Rejected };

private:
    int loanId = -1;
    int clientId = 0;
    std::string purpose = "";
    double requestedAmount = 0.0;
    int durationMonths = 0;
    double interestRate = 0.0;
    double guaranteesOffered = 0.0;
    LoanStatus status = LoanStatus::Pending;
    double defaultProbability = 0.0;
    double recoveryRate = 0.0;

    // Results
    bool resultCalculated = false;
    bool defaulted = false;
    double finalAmount = 0.0;

    //Unutilized
    enum class RiskLevel { Low, Medium, High, VeryHigh };
    RiskLevel realRisk = RiskLevel::Low;
    std::string internalRating = "";
    double remainingAmount = 0.0;
    std::string decisionReason = "";

public:
    // Constructors
    Loan() = default;

    Loan(int lId, int cId, const std::string& p, double ra, int dm, double ir, double go, LoanStatus loanStatus, double dp, double rr, RiskLevel risk, const std::string& rating, double remaining, const std::string& reason)
        : loanId(lId), clientId(cId), purpose(p), requestedAmount(ra), durationMonths(dm), interestRate(ir), guaranteesOffered(go), status(loanStatus), defaultProbability(dp), recoveryRate(rr), realRisk(risk), internalRating(rating), remainingAmount(remaining), decisionReason(reason) {}

    // Getter functions
    int getLoanId() const { return loanId; }
    int getClientId() const { return clientId; }
    const std::string& getPurpose() const { return purpose; }
    double getRequestedAmount() const { return requestedAmount; }
    int getDurationMonths() const { return durationMonths; }
    double getInterestRate() const { return interestRate; }
    double getGuaranteesOffered() const { return guaranteesOffered; }
    LoanStatus getStatus() const { return status; }
    double getDefaultProbability() const { return defaultProbability; }
    double getRecoveryRate() const { return recoveryRate; }
    RiskLevel getRealRisk() const { return realRisk; }
    const std::string& getInternalRating() const { return internalRating; }
    double getRemainingAmount() const { return remainingAmount; }
    const std::string& getDecisionReason() const { return decisionReason; }
    double getFinalAmount() const { return finalAmount; }

    // Status functions
    bool isPending() const { return status == LoanStatus::Pending; }
    bool isApproved() const { return status == LoanStatus::Approved; }
    bool isRejected() const { return status == LoanStatus::Rejected; }
    bool hasBeenEvaluated() const { return status != LoanStatus::Pending; }
    bool isResultCalculated() const { return resultCalculated; }
    bool hasDefaulted() const { return defaulted; }

    // Setter functions
    void setStatus(LoanStatus newStatus) { status = newStatus; }
    void setStatusToApproved() { status = LoanStatus::Approved; }
    void setStatusToPending() { status = LoanStatus::Pending; }
    void setStatusToRejected() { status = LoanStatus::Rejected; }
    void setDecisionReason(const std::string& reason) { decisionReason = reason; }
    void setNoReasonDecision() { decisionReason = "Personal choice"; }
    void setFinalAmount(double final) { finalAmount = final; }

    // Result computation functions
    bool computeDefaultEvent() const {
        if (!isApproved()) {
            return false;
        }

        int randomNumber = GetRandomValue(0, 9999);
        int defaultThreshold = static_cast<int>(defaultProbability * 10000.0);

        return randomNumber < defaultThreshold;
    }

    double computeFinalAmount() const {
        if (!isApproved()) {
            return 0.0;
        }

        if (defaulted) {
            return requestedAmount * recoveryRate;
        }

        double durationYears = durationMonths / 12.0;
        double interest = requestedAmount * interestRate * durationYears;

        return requestedAmount + interest;
    }

    double computeProfitOrLoss() const {
        if (!isApproved() || !resultCalculated) {
            return 0.0;
        }

        return finalAmount - requestedAmount;
    }

    void computeFinalResult() {
        if (resultCalculated) {
            return;
        }

        resultCalculated = true;
        defaulted = false;
        setFinalAmount(0.0);

        if (!isApproved()) {
            return;
        }

        defaulted = computeDefaultEvent();
        setFinalAmount(computeFinalAmount());
    }

    static LoanStatus stringToLoanStatus(const std::string& value) {
        if (value == "approved") {
            return LoanStatus::Approved;
        }

        if (value == "rejected") {
            return LoanStatus::Rejected;
        }

        return LoanStatus::Pending;
    }

    static const char* loanStatusToString(LoanStatus status) {
        if (status == LoanStatus::Approved) {
            return "approved";
        }

        if (status == LoanStatus::Rejected) {
            return "rejected";
        }

        return "pending";
    }

    static RiskLevel stringToRiskLevel(const std::string& value) {
        if (value == "medium") {
            return RiskLevel::Medium;
        }

        if (value == "high") {
            return RiskLevel::High;
        }

        if (value == "very_high") {
            return RiskLevel::VeryHigh;
        }

        return RiskLevel::Low;
    }

    static const char* riskLevelToString(RiskLevel risk) {
        if (risk == RiskLevel::Medium) {
            return "medium";
        }

        if (risk == RiskLevel::High) {
            return "high";
        }

        if (risk == RiskLevel::VeryHigh) {
            return "very_high";
        }

        return "low";
    }
};