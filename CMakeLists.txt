cmake_minimum_required(VERSION 3.20)

project(Project LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(PROJECT_SOURCES
    src/main.cpp
    src/Currency.cpp
    src/CurrencyHistory.cpp
    src/ExchangeShop.cpp
    src/Transaction.cpp
    src/Exchange.cpp
    src/Deposit.cpp
    src/Withdraw.cpp
    src/User.cpp
    src/Owner.cpp
    src/Cashier.cpp
    src/Client.cpp
    src/extra.cpp
)

add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE headers)
