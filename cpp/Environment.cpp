#include <stdexcept>
#include "Environment.h"
#include "utils.h"
#include "RuntimeError.h"
Environment::Environment():m_enclosing(nullptr) { 
    // LOG_DEBUG("create env! addr : ");
    std::cout << "create env = " << this << "no parent\n";
}

Environment::Environment(Environment* enclosing, Environment* globalEnv)
: m_enclosing(enclosing) 
, m_global(globalEnv)
{

    std::cout << "create env = " << this << " parent = " <<  enclosing <<"\n";
}

Environment::~Environment() {
    auto ptr =  reinterpret_cast<uint64_t>(this);
    LOG_DEBUG("Environment tear down, ptr = {}", ptr);
    std::cout << this << "\n";
}
void Environment::define(const std::string& name, std::unique_ptr<CppLoxCallable> function) {
    if (m_global != this) {
        m_global->define(name, std::move(function));
    }else {
        m_values[name] = function.get(); 
        std::cout << "define func, func ptr = " << function.get() << "\n";
        m_functions.push_back(std::move(function));
        std::cout << this << "\n";
        LOG_DEBUG("define function, name[{}]",  name);
    }
}

void Environment::define(const std::string& name, const Object& value) {
    m_values[name] = value;
    std::cout << this << "\n";
    LOG_DEBUG("define var, name[{}], value[{}]",  name, LoxLiteralTyeToString(value));
}

void Environment::assign(const Token& name, const Object& value) {
    if (m_values.count(name.m_lexeme) != 0) {
        m_values[name.m_lexeme] = value;
        return;
    }
    if (m_enclosing != nullptr)
    {
        m_enclosing->assign(name, value);
        return;
    }
    throw RuntimeError(name, "Undefined variable '" + name.m_lexeme + "'.");
}

Object Environment::get(Token token) {
    std::cout << this << "\n";
    LOG_DEBUG("env get name[{}]",  token.m_lexeme);
    if (m_values.count(token.m_lexeme) != 0) {
        LOG_DEBUG("     Find in current env");
        if (std::holds_alternative<nullptr_t>(m_values[token.m_lexeme])) {
            LOG_DEBUG("         Find in current env nullll");
        }
        return m_values[token.m_lexeme];
    }

    if (m_enclosing != nullptr)
    {
        LOG_DEBUG("     Find in parent env");
        if (std::holds_alternative<nullptr_t>(m_values[token.m_lexeme])) {
            LOG_DEBUG("         Find in current env nullll");
        }
        return m_enclosing->get(token);
    }
    throw RuntimeError(token, std::format("Undefined Variable '{}'.", token.m_lexeme));
}