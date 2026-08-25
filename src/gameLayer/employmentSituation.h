#pragma once

#include <string>

enum class EmploymentType { Determinato, Indeterminato };

class EmploymentSituation {
private:
    int employmentId = 0;
    int clientId = 0;
    std::string profession = "";
    EmploymentType employmentType = EmploymentType::Indeterminato;
    double monthlyIncome = 0.0;
    double yearsAtJob = 0.0;

public:
    EmploymentSituation() = default;

    EmploymentSituation(int eid, int cId, const std::string& p, EmploymentType et, double mi, double yaj)
        : employmentId(eid), clientId(cId), profession(p), employmentType(et), monthlyIncome(mi), yearsAtJob(yaj)
    {}

    int getEmploymentId() const {
        return employmentId;
    }

    int getClientId() const {
        return clientId;
    }

    const std::string& getProfession() const {
        return profession;
    }

    EmploymentType getEmploymentType() const {
        return employmentType;
    }

    double getMonthlyIncome() const {
        return monthlyIncome;
    }

    double getYearsAtJob() const {
        return yearsAtJob;
    }

    static EmploymentType stringToEmploymentType(const std::string& value) {
        if (value == "indeterminato") {
            return EmploymentType::Indeterminato;
        }

        return EmploymentType::Determinato;
    }

    static const char* employmentTypeToString(EmploymentType type) {
        if (type == EmploymentType::Indeterminato) {
            return "indeterminato";
        }

        return "determinato";
    }
};