#include <stdexcept>
#include "Environment.h"
void Environment::define(std::string name, Object value) {
    m_values.emplace(name, value);
}

Object  Environment::get(Token token) {
    if (m_values.count(token.m_lexeme) != 0) {
        return m_values[token.m_lexeme];
    }
    throw std::runtime_error(std::format("Undefined Variable '{}'.", token.m_lexeme));
}