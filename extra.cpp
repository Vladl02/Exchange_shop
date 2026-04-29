#include<extra.h>
#include<Exception.h>
#include<cctype>
#include<utility>

namespace {

bool isLeapYear(const int year) {
    if (year % 400 == 0) return true;
    if (year % 100 == 0) return false;
    return year % 4 == 0;
}

int getDaysInMonth(const int month, const int year) {
    if (month == 1 || month == 3 || month == 5 || month == 7 ||
        month == 8 || month == 10 || month == 12) {
        return 31;
    }
    if (month == 4 || month == 6 || month == 9 || month == 11) {
        return 30;
    }
    if (month == 2) return isLeapYear(year) ? 29 : 28;
    return 0;
}

} // namespace

Date::Date(int day_, int month_, int year_) : day(day_), month(month_), year(year_){
    if (year < 1) {
        throw ValidationException("year must be >= 1");
    }
    if (month < 1 || month > 12) {
        throw ValidationException("month must be in [1, 12]");
    }

    const int maxDay = getDaysInMonth(month, year);
    if (day < 1 || day > maxDay) {
        throw ValidationException("invalid day for given month/year");
    }
}

Date::Date(const std::string& iso_ymd) : day(1), month(1), year(1) {
    if (iso_ymd.size() != 10 || iso_ymd[4] != '-' || iso_ymd[7] != '-') {
        throw ValidationException("date must be in YYYY-MM-DD format");
    }

    for (int i = 0; i < 10; ++i) {
        if (i == 4 || i == 7) continue;
        if (!std::isdigit(static_cast<unsigned char>(iso_ymd[i]))) {
            throw ValidationException("date must be in YYYY-MM-DD format");
        }
    }

    const int parsedYear = std::stoi(iso_ymd.substr(0, 4));
    const int parsedMonth = std::stoi(iso_ymd.substr(5, 2));
    const int parsedDay = std::stoi(iso_ymd.substr(8, 2));
    *this = Date(parsedDay, parsedMonth, parsedYear);
}

std::string Date::stringify() const{
    return std::to_string(day) + "-" + std::to_string(month) + "-" + std::to_string(year);
}

bool operator==(const Date& lhs, const Date& rhs) {
    return lhs.day == rhs.day && lhs.month == rhs.month && lhs.year == rhs.year;
}

bool operator<(const Date& lhs, const Date& rhs) {
    if (lhs.year != rhs.year) return lhs.year < rhs.year;
    if (lhs.month != rhs.month) return lhs.month < rhs.month;
    return lhs.day < rhs.day;
}

bool operator<=(const Date& lhs, const Date& rhs) {
    return (lhs < rhs) || (lhs == rhs);
}



ExchangeRate::ExchangeRate(
    std::string currency_name_,
    double price_selling_,
    double price_buying_
) : currency_name(std::move(currency_name_)),
    price_selling(price_selling_),
    price_buying(price_buying_) {}


Record::Record(
        Date date_, 
        std::vector<ExchangeRate> rates_
):  date(std::move(date_)),
    rates(std::move(rates_)) {}

