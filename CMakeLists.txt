cmake_minimum_required(VERSION 3.20)

project(Project LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(PROJECT_SOURCES
    main.cpp
    Currency.cpp
    CurrencyHistory.cpp
    ExchangeShop.cpp
    Transaction.cpp
    Exchange.cpp
    Deposit.cpp
    Withdraw.cpp
    User.cpp
    Owner.cpp
    Cashier.cpp
    Client.cpp
    extra.cpp
)

add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE headers)
