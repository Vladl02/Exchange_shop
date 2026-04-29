#pragma once

#include <memory>
#include <ostream>
#include <string>
#include <vector>
#include <extra.h>

class ExchangeShop;
class Transaction;

class User {
protected:
    ExchangeShop* const shop;
private:
    std::string name;
    int id;
public:
    static int num_users;

    virtual ~User() = default;

    User(const std::string& name_, ExchangeShop& shop_);
    User(const User& other) = delete;
    User(User&& other) = delete;
    User& operator=(const User& other) = delete;
    User& operator=(User&& other) = delete;

    const std::string getName() const;
    int getId() const;

    virtual void printExpenseShopInfo(std::ostream& out) const = 0;
    virtual void printUserInfo(std::ostream& out) const = 0;

    friend std::ostream& operator<<(std::ostream& out, const User& user);

    std::vector<std::shared_ptr<Transaction>> getTransactionHistory(const Date& from) const;
    void printTransactionHistory(const Date& from, std::ostream& out) const;
private:
    virtual std::vector<std::shared_ptr<Transaction>> loadTransactionHistory(const Date& from) const;
    virtual std::vector<std::shared_ptr<Transaction>> filterTransactionHistory(
        const std::vector<std::shared_ptr<Transaction>>& history
    ) const = 0;

    virtual void outputTransactionHistory(
        const std::vector<std::shared_ptr<Transaction>>& history,
        std::ostream& out
    ) const;
};
