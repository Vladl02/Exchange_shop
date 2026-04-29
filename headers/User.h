#pragma once

#include <memory>
#include <ostream>
#include <string>
#include <vector>
#include <extra.h>

class ExchangeShop;
class Transaction;

// in .h avem doar declaratii de functii, implementarile sunt in .cpp
// aceasta este baza proprie pentru ierarhia user
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

    // functii virtuale pure
    // fiecare rol afiseaza alt tip de info despre casa de schimb
    virtual void printExpenseShopInfo(std::ostream& out) const = 0;
    virtual void printUserInfo(std::ostream& out) const = 0;

    friend std::ostream& operator<<(std::ostream& out, const User& user);

    // nvi
    // interfata publica e non-virtuala, iar pasii variabili sunt virtuali mai jos
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
