#ifndef RUNTIMEERROR_H
#define RUNTIMEERROR_H
#include <stdexcept>
#include <string>
#include "Token.h"

class RuntimeError : public std::runtime_error {
public:
    RuntimeError(const Token& token, const std::string& msg);
    RuntimeError() = delete;
    RuntimeError(const RuntimeError&) = delete;
    RuntimeError& operator=(const RuntimeError&) = delete;
    ~RuntimeError() = default;
private:
    Token m_token;
};

#endif // RUNTIMEERROR_H