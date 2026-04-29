#include <User.h>

#include <ExchangeShop.h>
#include <Exception.h>
#include <Transaction.h>

int User::num_users = 0;

std::ostream& operator<<(std::ostream& out, const User& user) {
    user.printUserInfo(out);
    return out;
}

User::User(const std::string& name_, ExchangeShop& shop_)
    : shop(&shop_), name(name_), id(num_users) {
    num_users++;
}

const std::string User::getName() const { return name; }
int User::getId() const { return id; }

std::vector<std::shared_ptr<Transaction>> User::getTransactionHistory(const Date& from) const {
    const std::vector<std::shared_ptr<Transaction>> history = loadTransactionHistory(from);
    return filterTransactionHistory(history);
}

void User::printTransactionHistory(const Date& from, std::ostream& out) const {
    const std::vector<std::shared_ptr<Transaction>> role_filtered = getTransactionHistory(from);
    outputTransactionHistory(role_filtered, out);
}

std::vector<std::shared_ptr<Transaction>> User::loadTransactionHistory(const Date& from) const {
    return shop->getTransactionHistory(from, Date(31, 12, 9999));
}

void User::outputTransactionHistory(
    const std::vector<std::shared_ptr<Transaction>>& history,
    std::ostream& out
) const {
    if (history.empty()) {
        out << "No transactions found.\n";
        return;
    }
    for (const auto& tx : history) out << *tx;
}
