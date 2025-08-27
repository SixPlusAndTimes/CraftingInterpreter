#include "CppLoxClass.h"

CppLoxClass::CppLoxClass(const std::string name)
: m_name(name) {

}

std::string CppLoxClass::toString() const{
    return m_name;
}

size_t CppLoxClass::arity() const {
    return 0;
}

Object CppLoxClass::call(Interpreter& interpreter, std::vector<Object>& arguments) {
    std::shared_ptr<CppLoxInstance> instance = std::make_shared<CppLoxInstance>(this);
    return instance;
}

