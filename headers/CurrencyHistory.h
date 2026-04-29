#pragma once
#include<vector>
#include<string>
#include<extra.h>
#include<Currency.h>


class CurrencyHistory{
private:
    std::string main_currency_name; // orice casa de schimb valutar are o valuta de baza in care arata preturile si cu care opereaza( in Romania de exemplu e RON)
    std::vector<Record> records;
public:
    
    explicit CurrencyHistory(const std::string main_currency_name_);

    double getSellingPrice(const Currency& currency, const Date& date) const;
    double getBuyingPrice(const Currency& currency, const Date& date) const;
    const Date getLastDate() const;
    const std::string getMainCurrencyName() const;
    
    void setCurrencyPrices(const Currency& currency, Date& date_, const double& priceBuy, const double& priceSell);

    bool verifyCurrency(const Currency& currency, const Date& date_) const;

    friend std::ostream& operator<<(std::ostream& out, const CurrencyHistory& history);

};
