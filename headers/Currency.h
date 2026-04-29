#pragma once
#include <string>

class Currency{
private:
    std::string name;
    double total;
public:
    Currency(const std::string name, const double& total);

    const std::string getName() const;
    double getTotal() const;

    void increaseTotal(const double& amount);
    void decreaseTotal(const double& amount);

    friend std::ostream& operator<<(std::ostream& out,const Currency& currency);
    Currency& operator=(const Currency& c) = default;
};