#include <Owner.h>

#include <ExchangeShop.h>
#include <Transaction.h>
#include <iomanip>

// aici se apeleaza constructorul din baza User
Owner::Owner(const std::string& name_, ExchangeShop& shop_) : User(name_, shop_) {}

void Owner::printExpenseShopInfo(std::ostream& out) const {
    out << "=== Exchange Shop Info (Owner) ===\n";
    out << "Main currency: " << shop->getMainCurrencyName() << '\n';
    out << "Tax: " << std::fixed << std::setprecision(2) << (shop->getTax() * 100.0) << "%\n";
    out << "Gross profit: " << std::fixed << std::setprecision(2) << shop->getGrossProfit() << '\n';
    out << "Net profit: " << std::fixed << std::setprecision(2) << shop->getNetProfit() << '\n';
    out << "Liquidity:\n";
    const std::vector<Currency>& currencies = shop->getCurrencies();
    for (const auto& c : currencies) {
        out << "- " << c.getName() << ": " << std::fixed << std::setprecision(2) << c.getTotal() << '\n';
    }
}

void Owner::printUserInfo(std::ostream& out) const {
    out << "----------- User -----------\n";
    out << "Id: " << getId() << '\n';
    out << "Name: " << getName() << '\n';
    out << "Role: Owner\n";
    out << "----------------------------\n";
}

void Owner::withdrawMoney(
    Date date_,
    const std::string& currency_name,
    const double amount
) const {
    shop->withdrawMoney(date_, getName(), getId(), currency_name, amount);
}

void Owner::depositMoney(
    Date date_,
    const std::string& currency_name,
    const double amount
) const {
    shop->depositMoney(date_, getName(), getId(), currency_name, amount);
}

const double Owner::getGrossProfit() const {
    return shop->getGrossProfit();
}

const double Owner::getNetProfit() const {
    return shop->getNetProfit();
}

void Owner::printLiquidity(std::ostream& out) const {
    const std::vector<Currency>& currencies = shop->getCurrencies();
    if (currencies.empty()) {
        out << "No currencies in shop.\n";
        return;
    }

    out << "Liquidity:\n";
    for (const auto& c : currencies) {
        out << "- " << c.getName()
            << ": " << std::fixed << std::setprecision(2)
            << c.getTotal() << '\n';
    }
}

std::vector<std::shared_ptr<Transaction>> Owner::filterTransactionHistory(
    const std::vector<std::shared_ptr<Transaction>>& history
) const {
    return history;
}

void Owner::outputTransactionHistory(
    const std::vector<std::shared_ptr<Transaction>>& history,
    std::ostream& out
) const {
    if (history.empty()) {
        out << "No transactions found.\n";
        return;
    }

    out << "=== Owner Transactions ===\n";
    for (const auto& tx : history) {
        out << *tx;
        out << "Gross profit: "
            << std::fixed << std::setprecision(2)
            << shop->getTransactionGrossProfit(*tx) << '\n';
        out << "Net profit: "
            << std::fixed << std::setprecision(2)
            << shop->getTransactionNetProfit(*tx) << "\n\n";
    }
}
