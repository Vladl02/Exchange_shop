#include<ExchangeShop.h>
#include<Exception.h>
#include<Exchange.h>
#include<Deposit.h>
#include<Withdraw.h>
#include<iostream>
#include<iomanip>
#include<memory>

ExchangeShop::ExchangeShop(const std::string main_currency_name_)
: history(main_currency_name_), main_currency_name(main_currency_name_), tax(0.0) { currencies.push_back(Currency(main_currency_name_, 0));}

ExchangeShop::ExchangeShop(const ExchangeShop& other)
 :  history(other.history), main_currency_name(std::move(other.main_currency_name)), tax(other.tax), currencies(other.currencies){
    for(auto& el: other.transactions){
        transactions.emplace_back(el->clone());
    }
}



void swap(ExchangeShop& s1, ExchangeShop& s2){
    std::swap(s1.currencies, s2.currencies);
    std::swap(s1.main_currency_name, s2.main_currency_name);
    std::swap(s1.tax, s2.tax);
    std::swap(s1.history, s2.history);
    std::swap(s1.transactions, s2.transactions);
}

ExchangeShop& ExchangeShop::operator=(ExchangeShop other){
    swap(*this, other);
    return (*this);
}

std::ostream& operator<<(std::ostream& out, const ExchangeShop& shop) {
    out << "------------ Exchange Shop ------------\n";
    out << "Main currency: " << shop.main_currency_name << '\n';
    out << "Tax: " << std::fixed << std::setprecision(2) << (shop.tax * 100.0) << "%\n";
    out << "Currencies tracked: " << shop.currencies.size() << '\n';
    out << "Transactions: " << shop.transactions.size() << '\n';
    out << "Last history date: " << shop.history.getLastDate().stringify() << '\n';
    out << "---------------------------------------\n";
    return out;
}



Currency* ExchangeShop::findCurrency(const std::string currency_name){
    for(auto &obj: currencies){
        if (currency_name.compare(obj.getName()) == 0) return &obj;
    }
    return nullptr;
}

const Currency* ExchangeShop::findCurrency(const std::string currency_name) const{
    for(const auto &obj: currencies){
        if (currency_name.compare(obj.getName()) == 0) return &obj;
    }
    return nullptr;
}

const Date ExchangeShop::getLastDate() const{ return history.getLastDate();}
const std::string ExchangeShop::getMainCurrencyName() const{ return history.getMainCurrencyName(); }

const double ExchangeShop::getMainCurrencyTotal() const{
    const Currency* curr = findCurrency(main_currency_name);
    if (curr == nullptr) {
        throw NotFoundException("main currency '" + main_currency_name + "' not found in shop totals");
    }
    return curr->getTotal();
}

const double ExchangeShop::getSellingPrice(const std::string currency_name, const Date date) const{
    const Currency* curr = findCurrency(currency_name);
    if (curr == nullptr) throw NotFoundException("currency '" + currency_name + "' not found");
    if (!history.verifyCurrency(*curr, date)) {
        throw NotFoundException("selling price for '" + currency_name + "' not found at requested date");
    }
    return history.getSellingPrice(*curr, date);
}

const double ExchangeShop::getBuyingPrice(const std::string currency_name, const Date date) const{
    const Currency* curr = findCurrency(currency_name);
    if (curr == nullptr) throw NotFoundException("currency '" + currency_name + "' not found");
    if (!history.verifyCurrency(*curr, date)) {
        throw NotFoundException("buying price for '" + currency_name + "' not found at requested date");
    }
    return history.getBuyingPrice(*curr, date);
}

const double ExchangeShop::getGrossProfit() const{
    double profit = 0.0;
    for (const auto& tx : transactions) {
        profit += tx->getGrossProfit(history);
    }
    return profit;
}

const double ExchangeShop::getNetProfit() const{
    double profit = 0.0;
    for (const auto& tx : transactions) {
        profit += tx->getNetProfit(history, tax);
    }
    return profit;
}

double ExchangeShop::getTransactionGrossProfit(const Transaction& transaction) const {
    return transaction.getGrossProfit(history);
}

double ExchangeShop::getTransactionNetProfit(const Transaction& transaction) const {
    return transaction.getNetProfit(history, tax);
}

const std::vector<Currency>& ExchangeShop::getCurrencies() const {
    return currencies;
}



double ExchangeShop::getTax() const { return tax; }

void ExchangeShop::setTax(const double tax_) {
    if (tax_ < 0.0 || tax_ > 1.0) {
        throw ValidationException("tax must be in [0, 1]");
    }
    tax = tax_;
}

void ExchangeShop::withdrawMoney(Date date_, std::string initiator_name_, int initiator_id_, const std::string currency_name, const double amount){
    Currency* curr = findCurrency(currency_name);
    if (amount <= 0) throw ValidationException("withdraw amount must be > 0");
    if (curr == nullptr) throw NotFoundException("currency '" + currency_name + "' not found");
    if (curr->getTotal() < amount) throw TransactionConstraintException("insufficient funds for withdraw in '" + currency_name + "'");

    curr->decreaseTotal(amount);

    std::shared_ptr<Transaction> transaction = Withdraw(date_, initiator_name_, initiator_id_, currency_name, amount).clone();
    transaction->setTransactionState(history, tax);
    transactions.push_back(transaction);
}

void ExchangeShop::depositMoney(Date date_, std::string initiator_name_, int initiator_id_, const std::string currency_name, const double amount){
    Currency* curr = findCurrency(currency_name);
    if (amount < 0) throw ValidationException("deposit amount cannot be negative");
    if (curr == nullptr) {currencies.push_back(Currency(currency_name, amount));}
    else {curr->increaseTotal(amount);}

    std::shared_ptr<Transaction> transaction = Deposit(date_, initiator_name_, initiator_id_, currency_name, amount).clone();
    transaction->setTransactionState(history, tax);
    transactions.push_back(transaction);
}

void ExchangeShop::exchangeMoney(Date date_, std::string initiator_name_, int initiator_id_, const std::string currency_in_name, const std::string currency_out_name, const double sum_in, double& sum_out){
    Currency* currency_in = findCurrency(currency_in_name);
    Currency* currency_out = findCurrency(currency_out_name);

    if (currency_in == nullptr) throw NotFoundException("currency_in '" + currency_in_name + "' not found");
    if (currency_out == nullptr) throw NotFoundException("currency_out '" + currency_out_name + "' not found");
    if (sum_in <= 0) throw ValidationException("exchange sum_in must be > 0");

    double payment = 0.0;

    if (currency_in_name.compare(history.getMainCurrencyName()) == 0){
        if (!history.verifyCurrency(*currency_out, date_)) {
            throw NotFoundException("missing price history for '" + currency_out_name + "' at requested date");
        }

        const double buy_price = history.getBuyingPrice(*currency_out, date_);
        if (buy_price <= 0.0) throw TransactionConstraintException("invalid buying price for '" + currency_out_name + "'");
        payment = sum_in / buy_price;

        if (payment <= currency_out->getTotal()){
            currency_in->increaseTotal(sum_in);
            currency_out->decreaseTotal(payment);
            sum_out = payment;
        } else {
            throw TransactionConstraintException("insufficient liquidity in '" + currency_out_name + "'");
        }
    } else if(currency_out_name.compare(history.getMainCurrencyName()) == 0){
        if (!history.verifyCurrency(*currency_in, date_)) {
            throw NotFoundException("missing price history for '" + currency_in_name + "' at requested date");
        }

        const double sell_price = history.getSellingPrice(*currency_in, date_);
        if (sell_price <= 0.0) throw TransactionConstraintException("invalid selling price for '" + currency_in_name + "'");
        payment = sum_in * sell_price;
        if (payment <= currency_out->getTotal()){
            currency_in->increaseTotal(sum_in);
            currency_out->decreaseTotal(payment);
            sum_out = payment;
        } else {
            throw TransactionConstraintException("insufficient liquidity in '" + currency_out_name + "'");
        }
    } else {
        if (!history.verifyCurrency(*currency_in, date_) || !history.verifyCurrency(*currency_out, date_)){
            throw NotFoundException("missing price history for one or both currencies at requested date");
        }

        const double sell_price_in = history.getSellingPrice(*currency_in, date_);
        if (sell_price_in <= 0.0) throw TransactionConstraintException("invalid selling price for '" + currency_in_name + "'");
        const double buy_price_out = history.getBuyingPrice(*currency_out, date_);
        if (buy_price_out <= 0.0) throw TransactionConstraintException("invalid buying price for '" + currency_out_name + "'");

        double payment_intermid = sum_in * sell_price_in;
        payment = payment_intermid / buy_price_out;

        if (payment <= currency_out->getTotal()){
            currency_in->increaseTotal(sum_in);
            currency_out->decreaseTotal(payment);
            sum_out = payment;
        } else {
            throw TransactionConstraintException("insufficient liquidity in '" + currency_out_name + "'");
        }
    }

    std::shared_ptr<Transaction> transaction = Exchange(
        date_,
        initiator_name_,
        initiator_id_,
        currency_in_name,
        currency_out_name,
        sum_in,
        sum_out
    ).clone();
    transaction->setTransactionState(history, tax);
    transactions.push_back(transaction);
}

void ExchangeShop::setPrice(const std::string currency_name, Date& date_, const double& priceBuy, const double& priceSell){
    if (currency_name.empty()) throw ValidationException("currency name cannot be empty");
    if (priceBuy <= 0.0 || priceSell <= 0.0) {
        throw ValidationException("buy/sell price must be > 0");
    }
    if (priceBuy < priceSell) {
        throw ValidationException("buy price must be >= sell price");
    }
    Currency* curr = findCurrency(currency_name);
    if (curr == nullptr) {
        currencies.push_back(Currency(currency_name, 0));
        curr = &currencies.back();
    }
    history.setCurrencyPrices(*curr, date_, priceBuy, priceSell);
}


void ExchangeShop::printTransactionStats(std::ostream& os){
    int exchanges=0, deposits=0, withdraws=0;
    for (auto& tr: transactions){
        if(auto tr1 = std::dynamic_pointer_cast<Exchange>(tr)) exchanges++;
        if(auto tr1 = std::dynamic_pointer_cast<Deposit>(tr)) deposits++;
        if(auto tr1 = std::dynamic_pointer_cast<Withdraw>(tr)) withdraws++;
    }

    os << "Exchange Transactions: " << exchanges << '\n';
    os << "Deposit Transactions: " << deposits << '\n';
    os << "Withdraw Transactions: " << withdraws << '\n';
}

std::vector<std::shared_ptr<Transaction>> ExchangeShop::getTransactionHistory(const Date& from, const Date& to) const {
    if (to < from) {
        throw ValidationException("from date cannot be after to date");
    }
    std::vector<std::shared_ptr<Transaction>> filtered;
    for (const auto& t : transactions) {
        const Date tx_date = t->getDate();
        if (from <= tx_date && tx_date <= to) filtered.push_back(t);
    }
    return filtered;
}
