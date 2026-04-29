#include <Deposit.h>

#include <CurrencyHistory.h>
#include <iomanip>

Deposit::Deposit(
    Date date_,
    std::string initiator_name_,
    int initiator_id_,
    std::string currency_name_,
    double sum_
) : Transaction(date_, initiator_name_, initiator_id_),
    currency_name(currency_name_),
    sum(sum_) {}

std::shared_ptr<Transaction> Deposit::clone() const { return std::make_shared<Deposit>(*this); }

const std::string Deposit::getCurrencyName() const { return currency_name; }
double Deposit::getSum() const { return sum; }

double Deposit::profitCalculation(const CurrencyHistory& history) const {
    (void)history;
    return 0.0;
}

void Deposit::afisare(std::ostream& out) const {
    out << '\n';
    out << "--------  Transaction info  ---------\n";
    out << "- Type: " << "Deposit" << '\n';
    out << "- id: " << getId() << '\n';
    out << "- Initiator name: " << getInitiatorName() << '\n';
    out << "- Initiator id: " << getInitiatorId() << '\n';
    out << "- Date: " << getDate().stringify() << '\n';
    out << "- Currency: " << getCurrencyName() << '\n';
    out << "- Money in: " << std::fixed << std::setprecision(2) << getSum() << " " << getCurrencyName() << '\n';
    if (!getTransactionState()) {
        out << "- Execution state: " << "Pending" << '\n';
    } else {
        out << "- Execution state: " << "Succesful" << '\n';
    }
    out << "-------------------------------------\n";
    out << '\n';
}
