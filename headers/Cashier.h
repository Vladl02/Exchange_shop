#pragma once

#include <memory>
#include <string>
#include <vector>
#include <extra.h>
#include <User.h>

class Transaction;
class ExchangeShop;


class Cashier : public User {
private:
    std::string owner_name;
public:
    Cashier(const std::string& name_, ExchangeShop& shop_, const std::string& owner_name_);

    void printExpenseShopInfo(std::ostream& out) const override;
    void printUserInfo(std::ostream& out) const override;

    void setPrice(
        const std::string& currency_name,
        Date& date_,
        const double& priceBuy,
        const double& priceSell
    ) const;

    void exchangeMoney(
        Date date_,
        const std::string& currency_in_name,
        const std::string& currency_out_name,
        const double sum_in,
        double& sum_out
    ) const;
private:
    std::vector<std::shared_ptr<Transaction>> filterTransactionHistory(
        const std::vector<std::shared_ptr<Transaction>>& history
    ) const override;
    void outputTransactionHistory(
        const std::vector<std::shared_ptr<Transaction>>& history,
        std::ostream& out
    ) const override;
};
