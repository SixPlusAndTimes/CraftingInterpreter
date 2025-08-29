#include "CppLoxClass.h"
static const std::string INIT_METHOD_NAME = "init";
CppLoxClass::CppLoxClass(const std::string name)
: m_name(name) {

}

 
CppLoxClass::CppLoxClass(const std::string name, const std::unordered_map<std::string, std::shared_ptr<LoxFunction>>& methods)
: m_name(name)
, m_methods(methods)
{ }

std::string CppLoxClass::toString() const{
    return m_name;
}

size_t CppLoxClass::arity() const {
    auto iter = m_methods.find(INIT_METHOD_NAME);
    if (iter != m_methods.end()) {
        return iter->second->arity();
    }
    return 0;
}

Object CppLoxClass::call(Interpreter& interpreter, std::vector<Object>& arguments) {
    std::shared_ptr<CppLoxInstance> instance = std::make_shared<CppLoxInstance>(this);
    if (m_methods.count(INIT_METHOD_NAME) != 0) {
        std::shared_ptr<LoxFunction> initializer = m_methods[INIT_METHOD_NAME];
        initializer->bind(instance)->call(interpreter, arguments);
    }
    return instance;
}


std::shared_ptr<LoxFunction> CppLoxClass::findMethod(std::string name) {
    if (m_methods.count(name) != 0) {
        return m_methods[name];
    }
    return nullptr;
}