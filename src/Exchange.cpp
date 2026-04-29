#include <Exchange.h>

#include <Currency.h>
#include <CurrencyHistory.h>
#include <iomanip>

Exchange::Exchange(
    Date date_,
    std::string initiator_name_,
    int initiator_id_,
    std::string currency_in_name_,
    std::string currency_out_name_,
    double sum_in_,
    double sum_out_
) : Transaction(date_, initiator_name_, initiator_id_),
    currency_in_name(std::move(currency_in_name_)),
    currency_out_name(std::move(currency_out_name_)),
    sum_in(sum_in_),
    sum_out(sum_out_) {}

std::shared_ptr<Transaction> Exchange::clone() const { return std::make_shared<Exchange>(*this); }

double Exchange::getSumIn() const { return sum_in; }
double Exchange::getSumOut() const { return sum_out; }

const std::string Exchange::getCurrencyInName() const { return currency_in_name; }
const std::string Exchange::getCurrencyOutName() const { return currency_out_name; }

double Exchange::profitCalculation(const CurrencyHistory& history) const {
    const Date date = getDate();
    const std::string main_currency_name = history.getMainCurrencyName();

    if (currency_in_name == main_currency_name) {
        const Currency currency_out(currency_out_name, 0.0);
        const double sell_price = history.getSellingPrice(currency_out, date);
        if (sell_price <= 0.0) return 0.0;
        return sum_in - (sum_out * sell_price);
    }

    if (currency_out_name == main_currency_name) {
        const Currency currency_in(currency_in_name, 0.0);
        const double buy_price = history.getBuyingPrice(currency_in, date);
        if (buy_price <= 0.0) return 0.0;
        return (sum_in * buy_price) - sum_out;
    }

    const Currency currency_in(currency_in_name, 0.0);
    const Currency currency_out(currency_out_name, 0.0);
    const double buy_in = history.getBuyingPrice(currency_in, date);
    const double sell_out = history.getSellingPrice(currency_out, date);
    if (buy_in <= 0.0 || sell_out <= 0.0) return 0.0;
    return (sum_in * buy_in) - (sum_out * sell_out);
}

void Exchange::afisare(std::ostream& out) const {
    out << '\n';
    out << "--------  Transaction info  ---------\n";
    out << "- Type: " << "Currency Exchange" << '\n';
    out << "- id: " << getId() << '\n';
    out << "- Initiator name: " << getInitiatorName() << '\n';
    out << "- Initiator id: " << getInitiatorId() << '\n';
    out << "- Date: " << getDate().stringify() << '\n';
    out << "- Currencies: " << getCurrencyInName() << " -> " << getCurrencyOutName() << '\n';
    out << "- Money in: " << std::fixed << std::setprecision(2) << getSumIn() << " " << getCurrencyInName() << '\n';
    out << "- Money out: ";
    if (!getTransactionState()) {
        out << "??? " << getCurrencyOutName() << '\n';
        out << "- Execution state: " << "Pending" << '\n';
    } else {
        out << std::fixed << std::setprecision(2) << getSumOut() << " " << getCurrencyOutName() << '\n';
        out << "- Execution state: " << "Succesful" << '\n';
    }
    out << "-------------------------------------\n";
    out << '\n';
}
