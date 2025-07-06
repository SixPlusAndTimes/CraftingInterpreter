#include <stdexcept>
#include "Environment.h"
#include "utils.h"
#include "RuntimeError.h"
void Environment::define(const std::string& name, const Object& value) {
    m_values[name] = value;
    LOG_DEBUG("define var, name[{}], value[{}]", name, LoxLiteralTyeToString(value));
}

void Environment::assign(const Token& name, const Object& value) {
    if (m_values.count(name.m_lexeme) != 0) {
        m_values[name.m_lexeme] = value;
        return;
    }
    throw RuntimeError(name, "Undefined variable '" + name.m_lexeme + "'.");
}

Object  Environment::get(Token token) {
    if (m_values.count(token.m_lexeme) != 0) {
        return m_values[token.m_lexeme];
    }
    throw RuntimeError(token, std::format("Undefined Variable '{}'.", token.m_lexeme));
}

