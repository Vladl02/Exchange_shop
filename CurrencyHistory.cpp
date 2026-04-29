#include<CurrencyHistory.h>
#include<iomanip>
#include<utility>



CurrencyHistory::CurrencyHistory(const std::string main_currency_name_) : main_currency_name(std::move(main_currency_name_)){}



double CurrencyHistory::getBuyingPrice(const Currency& currency, const Date& date) const{
    for (int i=0; i<records.size(); i++){
        if (records[i].date == date){
            Record curr_record = records[i];
            for (int j=0; j< curr_record.rates.size() ; j++){
                if (curr_record.rates[j].currency_name.compare(currency.getName()) == 0){
                    return curr_record.rates[j].price_buying;
                }
            }
        }
    }
    return 0.0;
}

double CurrencyHistory::getSellingPrice(const Currency& currency, const Date& date) const{
    for (int i=0; i<records.size(); i++){
        if (records[i].date == date){
            Record curr_record = records[i];
            for (int j=0; j< curr_record.rates.size() ; j++){
                if (curr_record.rates[j].currency_name.compare(currency.getName()) == 0){
                    return curr_record.rates[j].price_selling;
                }
            }
        }
    }
    return 0.0;
}

const Date CurrencyHistory::getLastDate() const{
    if (records.size() >= 1) {
        return records[records.size()-1].date;
    } else {
        return Date(1, 1, 1);
    }
}

const std::string CurrencyHistory::getMainCurrencyName() const{
    return main_currency_name;
}

std::ostream& operator<<(std::ostream& out, const CurrencyHistory& history){
    out << "---------------  Currency History  ------------------\n";
    for (int i=0; i<history.records.size(); i++){
        Record curr_record = history.records[i];
        out << "### "<< "Date " << curr_record.date.stringify()<< " contains this prices:" << '\n';
        for (int j=0; j< curr_record.rates.size() ; j++){
            out << "-" << curr_record.rates[j].currency_name << "   "<< "buy: "  << std::fixed << std::setprecision(2) << curr_record.rates[j].price_buying << "   " << "sell: " << curr_record.rates[j].price_selling << '\n';
        }
    }
    out << "-----------------------------------------------------\n";
    out << '\n';
    return out;
}


void CurrencyHistory::setCurrencyPrices(const Currency& currency, Date& date_, const double& priceBuy, const double& priceSell){
    std::string currencyName = currency.getName();
    Date date = date_;
    for (int i=0; i<records.size(); i++){
        if (records[i].date == date){
            for (int j=0; j< records[i].rates.size() ; j++){
                if (records[i].rates[j].currency_name.compare(currencyName) == 0){
                    records[i].rates[j].price_buying = priceBuy;
                    records[i].rates[j].price_selling = priceSell;
                    return;
                }
            }

            records[i].rates.push_back(ExchangeRate(currency.getName(), priceSell, priceBuy));
            return;
        }
    }

    records.push_back(
        Record(date_, 
        std::vector<ExchangeRate>{ExchangeRate(currency.getName(), priceSell, priceBuy)}));

}

bool CurrencyHistory::verifyCurrency(const Currency& currency, const Date& date_) const{
    for (int i=0; i<records.size(); i++){
        if (records[i].date == date_){
            Record passing_record = records[i];
            for (int j=0; j< passing_record.rates.size() ; j++){
                if (passing_record.rates[j].currency_name.compare(currency.getName()) == 0) return true;
            }
        }
    }
    return false;
}
