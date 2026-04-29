#include <Transaction.h>

#include <CurrencyHistory.h>

int Transaction::num_transactions = 0;

Transaction::Transaction(Date date_, std::string initiator_name_, int initiator_id_)
 : date(std::move(date_)),
   initiator_name(std::move(initiator_name_)),
   initiator_id(initiator_id_),
   id(num_transactions),
   executed(0),
   gross_profit(0.0),
   net_profit(0.0) {
    num_transactions++;
}

const std::string Transaction::getInitiatorName() const { return initiator_name; }
int Transaction::getInitiatorId() const { return initiator_id; }
const Date Transaction::getDate() const { return date; }
bool Transaction::getTransactionState() const { return executed; }
int Transaction::getId() const { return id; }

double Transaction::getGrossProfit(const CurrencyHistory& history) const {
    (void)history;
    if (!executed) return 0.0;
    return gross_profit;
}

double Transaction::getNetProfit(const CurrencyHistory& history, const double tax) const {
    (void)history;
    (void)tax;
    if (!executed) return 0.0;
    return net_profit;
}

void Transaction::setTransactionState(const CurrencyHistory& history, double tax) {
    if (executed) return;

    executed = 1;
    gross_profit = profitCalculation(history);

    if (gross_profit <= 0.0) {
        net_profit = gross_profit;
        return;
    }

    double clamped_tax = tax;
    if (clamped_tax < 0.0) clamped_tax = 0.0;
    if (clamped_tax > 1.0) clamped_tax = 1.0;
    net_profit = gross_profit * (1.0 - clamped_tax);
}

std::ostream& operator<<(std::ostream& out, const Transaction& transaction_) {
    transaction_.afisare(out);
    return out;
}
