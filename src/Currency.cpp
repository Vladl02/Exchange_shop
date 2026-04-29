
#include<Currency.h>
#include<string>
#include<iomanip>
#include<utility>


Currency::Currency(std::string name_, const double& total_) 
 :  name(std::move(name_)), total(total_){}


const std::string Currency::getName() const { return name; }
double Currency::getTotal() const { return total; }

void Currency::increaseTotal(const double& amount){
    total = total + amount;
}
void Currency::decreaseTotal(const double& amount){
    total = total - amount;
}


std::ostream& operator<<(std::ostream& out, const Currency& currency){
    out << "Currency: " << currency.name << " Total: " << std::fixed << std::setprecision(2) << currency.total << '\n';
    return out;
}
