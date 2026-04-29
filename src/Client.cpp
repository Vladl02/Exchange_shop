#include <Client.h>

#include <ExchangeShop.h>
#include <Exception.h>
#include <Transaction.h>
#include <iomanip>


Client::Client(const std::string& name_, ExchangeShop& shop_) : User(name_, shop_) {}

void Client::printExpenseShopInfo(std::ostream& out) const {
    out << "=== Exchange Shop Info (Client) ===\n";
    out << "Main currency: " << shop->getMainCurrencyName() << '\n';
    const Date last_date = shop->getLastDate();
    out << "Rates date: " << last_date.stringify() << '\n';
    out << "Rates (Buy/Sell):\n";
    const std::vector<Currency>& currencies = shop->getCurrencies();
    for (const auto& c : currencies) {
        if (c.getName() == shop->getMainCurrencyName()) continue;
        try {
            const double buy = shop->getBuyingPrice(c.getName(), last_date);
            const double sell = shop->getSellingPrice(c.getName(), last_date);
            out << "- " << c.getName()
                << ": " << std::fixed << std::setprecision(2) << buy
                << " / " << std::fixed << std::setprecision(2) << sell << '\n';
        } catch (const ExchangeException&) {
        }
    }
}

void Client::printUserInfo(std::ostream& out) const {
    out << "----------- User -----------\n";
    out << "Id: " << getId() << '\n';
    out << "Name: " << getName() << '\n';
    out << "Role: Client\n";
    out << "----------------------------\n";
}

const double Client::getSellingPrice(
    const std::string& currency_name,
    const Date& date
) const {
    return shop->getSellingPrice(currency_name, date);
}

const double Client::getBuyingPrice(
    const std::string& currency_name,
    const Date& date
) const {
    return shop->getBuyingPrice(currency_name, date);
}

void Client::exchangeMoney(
    Date date_,
    const std::string& currency_in_name,
    const std::string& currency_out_name,
    const double sum_in,
    double& sum_out
) const {
    shop->exchangeMoney(
        date_,
        getName(),
        getId(),
        currency_in_name,
        currency_out_name,
        sum_in,
        sum_out
    );
}

std::vector<std::shared_ptr<Transaction>> Client::filterTransactionHistory(
    const std::vector<std::shared_ptr<Transaction>>& history
) const {
    std::vector<std::shared_ptr<Transaction>> own_history;
    for (const auto& tx : history) {
        if (tx->getInitiatorId() == getId()) own_history.push_back(tx);
    }
    return own_history;
}
