#pragma once

#include <memory>
#include <ostream>
#include <string>
#include <vector>
#include <extra.h>
#include <User.h>

class Transaction;
class ExchangeShop;

class Owner : public User {
public:
    Owner(const std::string& name_, ExchangeShop& shop_);

    void printExpenseShopInfo(std::ostream& out) const override;
    void printUserInfo(std::ostream& out) const override;

    void withdrawMoney(
        Date date_,
        const std::string& currency_name,
        const double amount
    ) const;

    void depositMoney(
        Date date_,
        const std::string& currency_name,
        const double amount
    ) const;

    const double getGrossProfit() const;
    const double getNetProfit() const;

private:
    std::vector<std::shared_ptr<Transaction>> filterTransactionHistory(
        const std::vector<std::shared_ptr<Transaction>>& history
    ) const override;
    void outputTransactionHistory(
        const std::vector<std::shared_ptr<Transaction>>& history,
        std::ostream& out
    ) const override;
};
