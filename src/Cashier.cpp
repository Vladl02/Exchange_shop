#include <Cashier.h>

#include <ExchangeShop.h>
#include <Transaction.h>
#include <iomanip>

// aici se apeleaza constructorul din baza User
Cashier::Cashier(const std::string& name_, ExchangeShop& shop_, const std::string& owner_name_)
    : User(name_, shop_), owner_name(owner_name_) {}

void Cashier::printExpenseShopInfo(std::ostream& out) const {
    out << "=== Exchange Shop Info (Cashier) ===\n";
    out << "Owner name: " << owner_name << '\n';
    out << "Main currency: " << shop->getMainCurrencyName() << '\n';
    out << "Tax: " << std::fixed << std::setprecision(2) << (shop->getTax() * 100.0) << "%\n";
    out << "Liquidity:\n";
    const std::vector<Currency>& currencies = shop->getCurrencies();
    for (const auto& c : currencies) {
        out << "- " << c.getName() << ": " << std::fixed << std::setprecision(2) << c.getTotal() << '\n';
    }
}

void Cashier::printUserInfo(std::ostream& out) const {
    out << "----------- User -----------\n";
    out << "Id: " << getId() << '\n';
    out << "Name: " << getName() << '\n';
    out << "Role: Cashier\n";
    out << "----------------------------\n";
}

void Cashier::setPrice(
    const std::string& currency_name,
    Date& date_,
    const double& priceBuy,
    const double& priceSell
) const {
    shop->setPrice(currency_name, date_, priceBuy, priceSell);
}

void Cashier::exchangeMoney(
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

std::vector<std::shared_ptr<Transaction>> Cashier::filterTransactionHistory(
    const std::vector<std::shared_ptr<Transaction>>& history
) const {
    return history;
}

void Cashier::outputTransactionHistory(
    const std::vector<std::shared_ptr<Transaction>>& history,
    std::ostream& out
) const {
    if (history.empty()) {
        out << "No transactions found.\n";
        return;
    }

    out << "=== Cashier Transactions ===\n";
    for (const auto& tx : history) {
        out << *tx;
    }
}
