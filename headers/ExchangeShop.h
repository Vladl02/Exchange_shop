#pragma once
#include<Currency.h>
#include<CurrencyHistory.h>
#include<Transaction.h>
#include<memory>


class ExchangeShop{
private:
    std::vector<Currency> currencies;
    std::string main_currency_name;
    double tax;
    CurrencyHistory history;
    // pointer la baza cu obiecte derivate
    // aici apelam functii virtuale prin pointer la baza
    std::vector<std::shared_ptr<Transaction>> transactions;

    const Currency* findCurrency(const std::string currency_name) const;
    Currency* findCurrency(const std::string currency_name);
public:

    ExchangeShop(const std::string main_currency_name_);
    ExchangeShop(const ExchangeShop& other);
    const Date getLastDate() const;
    const std::string getMainCurrencyName() const;

    const double getMainCurrencyTotal() const;
    const double getSellingPrice(const std::string currency_name, const Date date) const;
    const double getBuyingPrice(const std::string currency_name, const Date date) const;
    const double getGrossProfit() const;
    const double getNetProfit() const;
    double getTransactionGrossProfit(const Transaction& transaction) const;
    double getTransactionNetProfit(const Transaction& transaction) const;
    const std::vector<Currency>& getCurrencies() const;
    double getTax() const;

    void setTax(const double tax_);

    void withdrawMoney(Date date_, std::string initiator_name_, int initiator_id_, const std::string currency_name, const double amount);
    void depositMoney(Date date_, std::string initiator_name_, int initiator_id_, const std::string currency_name, const double amount);
    void exchangeMoney(Date date_, std::string initiator_name_, int initiator_id_, const std::string currency_in_name, const std::string currency_out_name, const double sum_in, double& sum_out); 

    void printTransactionStats(std::ostream& os);
    void setPrice(const std::string currency_name, Date& date_, const double& priceBuy, const double& priceSell);

    std::vector<std::shared_ptr<Transaction>> getTransactionHistory(const Date& from, const Date& to) const;

    // copy and swap pentru atribuire corecta
    ExchangeShop& operator=(ExchangeShop other);

    friend void swap(ExchangeShop& s1, ExchangeShop& s2);
    friend std::ostream& operator<<(std::ostream& out, const ExchangeShop& shop);
};
