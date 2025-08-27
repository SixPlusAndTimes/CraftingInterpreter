#include "CppLoxInstance.h"
#include "RuntimeError.h"
CppLoxInstance::CppLoxInstance(CppLoxClass* klass)
: m_klass(*klass)
{

}

std::string CppLoxInstance::toString() const {
    return m_klass.toString() + " instance";
}

Object CppLoxInstance::get(std::shared_ptr<Token> name) {
    if (m_fields.count(name->m_lexeme) != 0) {
        return m_fields[name->m_lexeme];
    }
    throw RuntimeError(*name, "Undefined property '" + name->m_lexeme + "'.");
}

void CppLoxInstance::set(std::shared_ptr<Token> name, const Object& value) {
    m_fields[name->m_lexeme] = value;
}