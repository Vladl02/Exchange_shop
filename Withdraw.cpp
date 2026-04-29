#include <Withdraw.h>

#include <CurrencyHistory.h>
#include <iomanip>

Withdraw::Withdraw(
    Date date_,
    std::string initiator_name_,
    int initiator_id_,
    std::string currency_name_,
    double sum_
) : Transaction(date_, initiator_name_, initiator_id_),
    currency_name(currency_name_),
    sum(sum_) {}

std::shared_ptr<Transaction> Withdraw::clone() const { return std::make_shared<Withdraw>(*this); }

const std::string Withdraw::getCurrencyName() const { return currency_name; }
double Withdraw::getSum() const { return sum; }

double Withdraw::profitCalculation(const CurrencyHistory& history) const {
    (void)history;
    return 0.0;
}

void Withdraw::afisare(std::ostream& out) const {
    out << '\n';
    out << "--------  Transaction info  ---------\n";
    out << "- Type: " << "Withdraw" << '\n';
    out << "- id: " << getId() << '\n';
    out << "- Client name: " << getInitiatorName() << '\n';
    out << "- Initiator id: " << getInitiatorId() << '\n';
    out << "- Date: " << getDate().stringify() << '\n';
    out << "- Currency: " << getCurrencyName() << '\n';
    out << "- Money Out: " << std::fixed << std::setprecision(2) << getSum() << " " << getCurrencyName() << '\n';
    if (!getTransactionState()) {
        out << "- Execution state: " << "Pending" << '\n';
    } else {
        out << "- Execution state: " << "Succesful" << '\n';
    }
    out << "-------------------------------------\n";
    out << '\n';
}
