#pragma once

#include <string>
#include <memory>
#include <Transaction.h>

class CurrencyHistory;

class Withdraw : public Transaction {
private:
    std::string currency_name;
    double sum;
public:
    Withdraw(
        Date date_,
        std::string initiator_name_,
        int initiator_id_,
        std::string currency_name_,
        double sum_
    );

    std::shared_ptr<Transaction> clone() const override;

    const std::string getCurrencyName() const;
    double getSum() const;

protected:
    void afisare(std::ostream& out) const override;
private:
    double profitCalculation(const CurrencyHistory& history) const override;
};
