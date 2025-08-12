#ifndef LOXFUNCTION_H
#define LOXFUNCTION_H
#include "CppLoxCallable.h"
#include "Stmt.h"
#include "Environment.h"
class Environment;
class LoxFunction : public CppLoxCallable {
    public:
                    LoxFunction(Function* declaration, const Environment& env);
        size_t      arity() const override;
        Object      call(Interpreter& interpreter, std::vector<Object>& arguments) override;
        std::string toString() const override;
    private:
        Function* m_declaration;
        Environment    m_closure;
};

#endif // LOXFUNCTION_H