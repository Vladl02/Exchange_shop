#pragma once
#include <stdexcept>
#include <string>

class ExchangeException : public std::runtime_error {
public:
    explicit ExchangeException(const std::string& message_)
        : std::runtime_error(message_) {}
};

class ValidationException : public ExchangeException {
public:
    explicit ValidationException(const std::string& message_)
        : ExchangeException("Validation error: " + message_) {}
};

class NotFoundException : public ExchangeException {
public:
    explicit NotFoundException(const std::string& message_)
        : ExchangeException("Not found: " + message_) {}
};

class TransactionConstraintException : public ExchangeException {
public:
    explicit TransactionConstraintException(const std::string& message_)
        : ExchangeException("Transaction constraint error: " + message_) {}
};


