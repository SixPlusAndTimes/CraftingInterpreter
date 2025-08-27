#include "CppLoxInstance.h"
CppLoxInstance::CppLoxInstance(CppLoxClass* klass)
: m_klass(*klass)
{

}

std::string CppLoxInstance::toString() const {
    return m_klass.toString() + " instance";
}