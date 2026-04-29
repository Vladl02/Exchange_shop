#include <ExchangeShop.h>
#include <Exception.h>
#include <Owner.h>
#include <Cashier.h>
#include <Client.h>

#include <fstream>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

namespace {

const std::string kDataFile = "shop_data.txt";

void clearScreen() {
#ifdef _WIN32
    std::system("cls");
#else
    std::system("clear");
#endif
}

void clearInputBuffer() {
    std::cin.clear();
    std::cin.ignore(10000, '\n');
}

std::string askToken(const std::string& prompt) {
    std::cout << prompt;
    std::string value;
    if (!(std::cin >> value)) {
        clearInputBuffer();
        throw ValidationException("expected text input");
    }
    return value;
}

int askInt(const std::string& prompt) {
    std::cout << prompt;
    int value = 0;
    if (!(std::cin >> value)) {
        clearInputBuffer();
        throw ValidationException("expected integer input");
    }
    return value;
}

double askDouble(const std::string& prompt) {
    std::cout << prompt;
    double value = 0.0;
    if (!(std::cin >> value)) {
        clearInputBuffer();
        throw ValidationException("expected numeric input");
    }
    return value;
}

Date askDate(const std::string& label) {
    return Date(askToken("Enter " + label + " (YYYY-MM-DD): "));
}

bool loadSetup(
    const std::string& path,
    std::string& mainCurrency,
    std::string& ownerName,
    std::string& cashierName
) {
    std::ifstream in(path);
    if (!in) return false;

    if (!std::getline(in, mainCurrency)) return false;
    if (!std::getline(in, ownerName)) return false;
    if (!std::getline(in, cashierName)) return false;

    return !(mainCurrency.empty() || ownerName.empty() || cashierName.empty());
}

bool saveSetup(
    const std::string& path,
    const std::string& mainCurrency,
    const std::string& ownerName,
    const std::string& cashierName
) {
    std::ofstream out(path, std::ios::trunc);
    if (!out) return false;

    out << mainCurrency << '\n';
    out << ownerName << '\n';
    out << cashierName << '\n';
    if (!out) return false;
    return true;
}

bool appendOperation(
    const std::string& path,
    const std::string& type,
    const std::vector<std::string>& fields
) {
    std::ofstream out(path, std::ios::app);
    if (!out) return false;

    out << type << '\n';
    for (const auto& field : fields) out << field << '\n';
    out << '\n';
    if (!out) return false;
    return true;
}

void replayOperations(const std::string& path, ExchangeShop& shop, Owner& owner, Cashier& cashier) {
    std::ifstream in(path);
    if (!in) return;

    std::string mainCurrency;
    std::string ownerName;
    std::string cashierName;
    if (!(in >> mainCurrency >> ownerName >> cashierName)) return;

    std::string type;
    while (in >> type) {
        try {
            if (type == "SET_TAX") {
                double tax = 0.0;
                if (!(in >> tax)) break;
                shop.setTax(tax);
            } else if (type == "SET_PRICE") {
                int day = 0, month = 0, year = 0;
                std::string currency;
                double buy = 0.0, sell = 0.0;
                if (!(in >> day >> month >> year >> currency >> buy >> sell)) break;

                Date date(day, month, year);
                cashier.setPrice(currency, date, buy, sell);
            } else if (type == "OWNER_DEPOSIT") {
                int day = 0, month = 0, year = 0;
                std::string currency;
                double amount = 0.0;
                if (!(in >> day >> month >> year >> currency >> amount)) break;

                owner.depositMoney(Date(day, month, year), currency, amount);
            } else if (type == "OWNER_WITHDRAW") {
                int day = 0, month = 0, year = 0;
                std::string currency;
                double amount = 0.0;
                if (!(in >> day >> month >> year >> currency >> amount)) break;

                owner.withdrawMoney(Date(day, month, year), currency, amount);
            } else if (type == "CASHIER_EXCHANGE") {
                int day = 0, month = 0, year = 0;
                std::string inCurrency;
                std::string outCurrency;
                double sumIn = 0.0;
                if (!(in >> day >> month >> year >> inCurrency >> outCurrency >> sumIn)) break;

                double sumOut = 0.0;
                cashier.exchangeMoney(Date(day, month, year), inCurrency, outCurrency, sumIn, sumOut);
            } else if (type == "USER_EXCHANGE") {
                std::string userName;
                int userId = 0, day = 0, month = 0, year = 0;
                std::string inCurrency;
                std::string outCurrency;
                double sumIn = 0.0;
                if (!(in >> userName >> userId >> day >> month >> year >> inCurrency >> outCurrency >> sumIn)) break;

                double sumOut = 0.0;
                shop.exchangeMoney(Date(day, month, year), userName, userId, inCurrency, outCurrency, sumIn, sumOut);
            } else {
                std::cout << "Unknown operation in saved data: " << type << '\n';
                break;
            }
        } catch (const std::exception& e) {
            std::cout << "Failed to replay operation '" << type << "': " << e.what() << '\n';
            break;
        }
    }
}

void syncNextUserId(const Owner& owner, const Cashier& cashier, const ExchangeShop& shop) {
    int maxId = owner.getId();
    if (cashier.getId() > maxId) maxId = cashier.getId();

    const auto history = shop.getTransactionHistory(Date(1, 1, 1), Date(31, 12, 9999));
    for (const auto& tx : history) {
        if (tx->getInitiatorId() > maxId) maxId = tx->getInitiatorId();
    }

    if (User::num_users <= maxId) User::num_users = maxId + 1;
}

void populateCurrencyHistory(Cashier& cashier, const std::string& mainCurrency) {
    try {
        if (mainCurrency != "EUR") {
            Date eurDay1(1, 1, 2024);
            Date eurDay2(2, 1, 2024);
            cashier.setPrice("EUR", eurDay1, 5.03, 4.93);
            cashier.setPrice("EUR", eurDay2, 5.05, 4.95);
        }

        if (mainCurrency != "USD") {
            Date usdDay1(1, 1, 2024);
            Date usdDay2(2, 1, 2024);
            cashier.setPrice("USD", usdDay1, 4.62, 4.52);
            cashier.setPrice("USD", usdDay2, 4.66, 4.55);
        }

        if (mainCurrency != "GBP") {
            Date gbpDay1(1, 1, 2024);
            Date gbpDay2(2, 1, 2024);
            cashier.setPrice("GBP", gbpDay1, 5.85, 5.73);
            cashier.setPrice("GBP", gbpDay2, 5.88, 5.76);
        }
    } catch (const ExchangeException&) {
    }
}

void printLiquidity(const ExchangeShop& shop) {
    const auto& currencies = shop.getCurrencies();
    if (currencies.empty()) {
        std::cout << "No currencies in shop.\n";
        return;
    }

    std::cout << "Liquidity:\n";
    for (const auto& currency : currencies) {
        std::cout << "- " << currency.getName()
                  << ": " << std::fixed << std::setprecision(2)
                  << currency.getTotal() << '\n';
    }
}

void ownerMenu(Owner& owner, ExchangeShop& shop) {
    while (true) {
        std::cout << "\n=== Owner Menu ===\n";
        std::cout << "1. Owner info\n";
        std::cout << "2. Deposit money\n";
        std::cout << "3. Withdraw money\n";
        std::cout << "4. Show gross/net profit\n";
        std::cout << "5. Set tax\n";
        std::cout << "6. Print transaction history (from date)\n";
        std::cout << "7. Get liquidity\n";
        std::cout << "8. Exchange shop info (owner view)\n";
        std::cout << "0. Back\n";

        int option = 0;
        try {
            option = askInt("Choose: ");
            clearScreen();
            if (option == 0) break;

            switch (option) {
            case 1: {
                std::cout << owner;
                break;
            }
            case 2: {
                const Date date = askDate("deposit date");
                const std::string currency = askToken("Currency: ");
                const double amount = askDouble("Amount: ");
                owner.depositMoney(date, currency, amount);

                const bool ok = appendOperation(kDataFile, "OWNER_DEPOSIT", {
                    std::to_string(date.day),
                    std::to_string(date.month),
                    std::to_string(date.year),
                    currency,
                    std::to_string(amount)
                });
                if (!ok) std::cout << "Warning: could not save operation to file.\n";
                std::cout << "Deposit successful.\n";
                break;
            }
            case 3: {
                const Date date = askDate("withdraw date");
                const std::string currency = askToken("Currency: ");
                const double amount = askDouble("Amount: ");
                owner.withdrawMoney(date, currency, amount);

                const bool ok = appendOperation(kDataFile, "OWNER_WITHDRAW", {
                    std::to_string(date.day),
                    std::to_string(date.month),
                    std::to_string(date.year),
                    currency,
                    std::to_string(amount)
                });
                if (!ok) std::cout << "Warning: could not save operation to file.\n";
                std::cout << "Withdraw successful.\n";
                break;
            }
            case 4:
                std::cout << "Gross profit: " << std::fixed << std::setprecision(2) << owner.getGrossProfit() << '\n';
                std::cout << "Net profit: " << std::fixed << std::setprecision(2) << owner.getNetProfit() << '\n';
                break;
            case 5: {
                const double tax = askDouble("Tax [0..1]: ");
                shop.setTax(tax);

                const bool ok = appendOperation(kDataFile, "SET_TAX", {std::to_string(tax)});
                if (!ok) std::cout << "Warning: could not save operation to file.\n";
                std::cout << "Tax updated.\n";
                break;
            }
            case 6: {
                const Date from = askDate("from");
                owner.printTransactionHistory(from, std::cout);
                break;
            }
            case 7:
                printLiquidity(shop);
                break;
            case 8:
                owner.printExpenseShopInfo(std::cout);
                break;
            default:
                std::cout << "Unknown option.\n";
                break;
            }
        } catch (const ExchangeException& e) {
            std::cout << e.what() << '\n';
        } catch (const std::exception& e) {
            std::cout << "Unexpected error: " << e.what() << '\n';
        }
    }
}

void cashierMenu(Cashier& cashier, ExchangeShop& shop) {
    while (true) {
        std::cout << "\n=== Cashier Menu ===\n";
        std::cout << "1. Cashier info\n";
        std::cout << "2. Set currency prices\n";
        std::cout << "3. Exchange money\n";
        std::cout << "4. Print transaction history (from date)\n";
        std::cout << "5. Get liquidity\n";
        std::cout << "6. Exchange shop info (cashier view)\n";
        std::cout << "0. Back\n";

        int option = 0;
        try {
            option = askInt("Choose: ");
            clearScreen();
            if (option == 0) break;

            switch (option) {
            case 1:
                std::cout << cashier;
                break;
            case 2: {
                const std::string currency = askToken("Currency: ");
                Date date = askDate("price date");
                const double buy = askDouble("Buy price: ");
                const double sell = askDouble("Sell price: ");
                cashier.setPrice(currency, date, buy, sell);

                const bool ok = appendOperation(kDataFile, "SET_PRICE", {
                    std::to_string(date.day),
                    std::to_string(date.month),
                    std::to_string(date.year),
                    currency,
                    std::to_string(buy),
                    std::to_string(sell)
                });
                if (!ok) std::cout << "Warning: could not save operation to file.\n";
                std::cout << "Price updated.\n";
                break;
            }
            case 3: {
                const Date date = askDate("exchange date");
                const std::string inCurrency = askToken("Currency in: ");
                const std::string outCurrency = askToken("Currency out: ");
                const double sumIn = askDouble("Sum in: ");
                double sumOut = 0.0;
                cashier.exchangeMoney(date, inCurrency, outCurrency, sumIn, sumOut);

                const bool ok = appendOperation(kDataFile, "CASHIER_EXCHANGE", {
                    std::to_string(date.day),
                    std::to_string(date.month),
                    std::to_string(date.year),
                    inCurrency,
                    outCurrency,
                    std::to_string(sumIn)
                });
                if (!ok) std::cout << "Warning: could not save operation to file.\n";

                std::cout << "Exchange successful. Sum out: "
                          << std::fixed << std::setprecision(2)
                          << sumOut << " " << outCurrency << '\n';
                break;
            }
            case 4: {
                const Date from = askDate("from");
                cashier.printTransactionHistory(from, std::cout);
                break;
            }
            case 5:
                printLiquidity(shop);
                break;
            case 6:
                cashier.printExpenseShopInfo(std::cout);
                break;
            default:
                std::cout << "Unknown option.\n";
                break;
            }
        } catch (const ExchangeException& e) {
            std::cout << e.what() << '\n';
        } catch (const std::exception& e) {
            std::cout << "Unexpected error: " << e.what() << '\n';
        }
    }
}

void userMenu(ExchangeShop& shop) {
    try {
        const std::string userName = askToken("User name: ");
        Client user(userName, shop);

        std::cout << "Logged in as user '" << user.getName() << "' with id=" << user.getId() << "\n";

        while (true) {
            std::cout << "\n=== User Menu (" << user.getName() << ", id=" << user.getId() << ") ===\n";
            std::cout << "1. User info\n";
            std::cout << "2. Get selling price (last date)\n";
            std::cout << "3. Get buying price (last date)\n";
            std::cout << "4. Exchange money (last date)\n";
            std::cout << "5. Print my transaction history (until last date)\n";
            std::cout << "6. Exchange shop info (client view)\n";
            std::cout << "0. Back\n";

            const int option = askInt("Choose: ");
            clearScreen();
            if (option == 0) break;

            const Date lastDate = shop.getLastDate();

            switch (option) {
            case 1:
                std::cout << user;
                break;
            case 2: {
                const std::string currency = askToken("Currency: ");
                std::cout << "Selling price: " << std::fixed << std::setprecision(2)
                          << user.getSellingPrice(currency, lastDate) << '\n';
                std::cout << "(date used: " << lastDate.stringify() << ")\n";
                break;
            }
            case 3: {
                const std::string currency = askToken("Currency: ");
                std::cout << "Buying price: " << std::fixed << std::setprecision(2)
                          << user.getBuyingPrice(currency, lastDate) << '\n';
                std::cout << "(date used: " << lastDate.stringify() << ")\n";
                break;
            }
            case 4: {
                const std::string inCurrency = askToken("Currency in: ");
                const std::string outCurrency = askToken("Currency out: ");
                const double sumIn = askDouble("Sum in: ");
                double sumOut = 0.0;
                user.exchangeMoney(lastDate, inCurrency, outCurrency, sumIn, sumOut);

                const bool ok = appendOperation(kDataFile, "USER_EXCHANGE", {
                    user.getName(),
                    std::to_string(user.getId()),
                    std::to_string(lastDate.day),
                    std::to_string(lastDate.month),
                    std::to_string(lastDate.year),
                    inCurrency,
                    outCurrency,
                    std::to_string(sumIn)
                });
                if (!ok) std::cout << "Warning: could not save operation to file.\n";

                std::cout << "Exchange successful. Sum out: "
                          << std::fixed << std::setprecision(2)
                          << sumOut << " " << outCurrency << '\n';
                std::cout << "(date used: " << lastDate.stringify() << ")\n";
                break;
            }
            case 5: {
                user.printTransactionHistory(Date(1, 1, 1), std::cout);
                break;
            }
            case 6:
                user.printExpenseShopInfo(std::cout);
                break;
            default:
                std::cout << "Unknown option.\n";
                break;
            }
        }
    } catch (const ExchangeException& e) {
        std::cout << e.what() << '\n';
    } catch (const std::exception& e) {
        std::cout << "Unexpected error: " << e.what() << '\n';
    }
}

} // namespace

int main() {
    std::cout << "=== Exchange Shop Console ===\n";
    bool interactive = true;

    std::string mainCurrency;
    std::string ownerName;
    std::string cashierName;

    if (!loadSetup(kDataFile, mainCurrency, ownerName, cashierName)) {
        std::cout << "No valid data file found. Initial setup required.\n";
        try {
            mainCurrency = askToken("Main currency: ");
            ownerName = askToken("Owner name: ");
            cashierName = askToken("Cashier name: ");
        } catch (const ExchangeException& e) {
            std::cout << e.what() << '\n';
            return 1;
        }

        if (!saveSetup(kDataFile, mainCurrency, ownerName, cashierName)) {
            std::cout << "Failed to create data file.\n";
            return 1;
        }
    } else {
        std::cout << "Loaded data from " << kDataFile << ".\n";
    }

    ExchangeShop shop(mainCurrency);
    Owner owner(ownerName, shop);
    Cashier cashier(cashierName, shop, ownerName);

    populateCurrencyHistory(cashier, mainCurrency);
    replayOperations(kDataFile, shop, owner, cashier);
    syncNextUserId(owner, cashier, shop);

    while (interactive) {
        std::cout << "\n=== Choose Role ===\n";
        std::cout << "a - Owner\n";
        std::cout << "b - Cashier\n";
        std::cout << "c - User\n";
        std::cout << "d - Exit\n";

        try {
            const std::string optionText = askToken("Introdu: ");
            clearScreen();
            const char option = optionText[0];

            switch (option) {
            case 'a':
                ownerMenu(owner, shop);
                break;
            case 'b':
                cashierMenu(cashier, shop);
                break;
            case 'c':
                userMenu(shop);
                break;
            case 'd':
                std::cout << "Bye.\n";
                interactive = false;
                break;
            default:
                std::cout << "Unknown option.\n";
                break;
            }
        } catch (const ExchangeException& e) {
            std::cout << e.what() << '\n';
        }
    }

    return 0;
}
