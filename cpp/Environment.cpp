#include <stdexcept>
#include "Environment.h"
#include "utils.h"
#include "RuntimeError.h"
void Environment::define(std::string name, Object value) {
    m_values[name] = value;
    LOG_DEBUG("define var, name[{}], value[{}]", name, LoxLiteralTyeToString(value));
    LOG_DEBUG("m_value[{}] = {}", name, LoxLiteralTyeToString(m_values[name]));
}

Object  Environment::get(Token token) {
    if (m_values.count(token.m_lexeme) != 0) {
        return m_values[token.m_lexeme];
    }
    throw RuntimeError(token, std::format("Undefined Variable '{}'.", token.m_lexeme));
}