#ifndef CPPLOXCLASS_H
#define CPPLOXCLASS_H
#include <string>
#include "CppLoxCallable.h"
#include "CppLoxInstance.h"
#include "Object.h"

class CppLoxClass : public CppLoxCallable {
    public:
                    CppLoxClass(const std::string name);
        size_t      arity() const override;
        std::string toString() const override;
        Object      call(Interpreter& interpreter, std::vector<Object>& arguments) override;
    private:
        std::string m_name;
};

#endif // CPPLOXCLASS_H