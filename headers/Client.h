#pragma once

#include <memory>
#include <string>
#include <vector>
#include <extra.h>
#include <User.h>

class Transaction;
class ExchangeShop;

class Client : public User {
public:
    Client(const std::string& name_, ExchangeShop& shop_);

    void printExpenseShopInfo(std::ostream& out) const override;
    void printUserInfo(std::ostream& out) const override;

    const double getSellingPrice(
        const std::string& currency_name,
        const Date& date
    ) const;

    const double getBuyingPrice(
        const std::string& currency_name,
        const Date& date
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
};
