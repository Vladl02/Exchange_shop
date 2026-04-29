#pragma once

#include <string>
#include <vector>


struct Date{
    int day;
    int month;
    int year;

    Date(int day_, int month_, int year_);
    explicit Date(const std::string& iso_ymd);

    std::string stringify() const;
};

bool operator==(const Date& lhs, const Date& rhs);
bool operator<(const Date& lhs, const Date& rhs);
bool operator<=(const Date& lhs, const Date& rhs);
bool cmp_date(const Date& d1, const Date& d2);

struct ExchangeRate{
    std::string currency_name;
    double price_selling;
    double price_buying;

    ExchangeRate(
        std::string currency_name_,
        double price_selling_,
        double price_buying_
    );
};

struct Record {
    Date date;
    std::vector<ExchangeRate> rates;

    Record(
        Date date_, 
        std::vector<ExchangeRate> rates_
    );

    Record(const Record &other) = default;
    Record &operator=(const Record &other) = default;
};
