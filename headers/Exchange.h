#pragma once

#include <string>
#include <memory>
#include <Transaction.h>

class CurrencyHistory;

class Exchange : public Transaction {
private:
    std::string currency_in_name;
    std::string currency_out_name;
    double sum_in;
    double sum_out;
public:
    Exchange(
        Date date_,
        std::string initiator_name_,
        int initiator_id_,
        std::string currency_in_name_,
        std::string currency_out_name_,
        double sum_in_,
        double sum_out_
    );

    std::shared_ptr<Transaction> clone() const override;

    const std::string getCurrencyInName() const;
    const std::string getCurrencyOutName() const;

    double getSumIn() const;
    double getSumOut() const;

protected:
    void afisare(std::ostream& out) const override;
private:
    double profitCalculation(const CurrencyHistory& history) const override;
};
