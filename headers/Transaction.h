#pragma once

#include <string>
#include <memory>
#include <extra.h>

class CurrencyHistory;

class Transaction {
private:
    std::string initiator_name;
    int initiator_id;
    Date date;
    bool executed;
    int id;
    double gross_profit;
    double net_profit;
    virtual double profitCalculation(const CurrencyHistory& history) const = 0;
public:
    Transaction(Date date_, std::string initiator_name_, int initiator_id_);

    virtual std::shared_ptr<Transaction> clone() const = 0;

    virtual ~Transaction() = default;
    static int num_transactions;

    const std::string getInitiatorName() const;
    int getInitiatorId() const;
    const Date getDate() const;
    bool getTransactionState() const;
    int getId() const;
    double getGrossProfit(const CurrencyHistory& history) const;
    double getNetProfit(const CurrencyHistory& history, const double tax) const;

    void setTransactionState(const CurrencyHistory& history, double tax);

    friend std::ostream& operator<<(std::ostream& out, const Transaction& transaction);

    Transaction(Transaction&& other) = default;
    Transaction& operator=(Transaction&& other) = default;
protected:
    Transaction(const Transaction& other) = default;
    Transaction& operator=(const Transaction& other) = default;
private:
    virtual void afisare(std::ostream& out) const = 0;
};
