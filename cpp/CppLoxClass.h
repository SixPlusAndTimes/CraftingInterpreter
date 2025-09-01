#ifndef CPPLOXCLASS_H
#define CPPLOXCLASS_H
#include <string>
#include "CppLoxCallable.h"
#include "CppLoxInstance.h"
#include "Object.h"
#include <unordered_map>
#include "LoxFunction.h"

class CppLoxClass : public CppLoxCallable {
    public:
                    CppLoxClass(const std::string name);
                    CppLoxClass(const std::string name, std::shared_ptr<CppLoxClass> superclass, const std::unordered_map<std::string, std::shared_ptr<LoxFunction>>& methods);
        size_t      arity() const override;
        std::string toString() const override;
        Object      call(Interpreter& interpreter, std::vector<Object>& arguments) override;
        std::shared_ptr<LoxFunction> findMethod(const std::string& name);
    private:
        std::string m_name;
        std::shared_ptr<CppLoxClass> m_superClass;
        std::unordered_map<std::string, std::shared_ptr<LoxFunction>> m_methods;
};

#endif // CPPLOXCLASS_H