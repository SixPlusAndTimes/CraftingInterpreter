#ifndef LOXFUNCTION_H
#define LOXFUNCTION_H

#include "CppLoxCallable.h"
#include "Stmt.h"
#include "Environment.h"
class Environment;
class LoxFunction : public CppLoxCallable {
    public:
                    LoxFunction(Function* declaration, std::shared_ptr<Environment> env, bool isInitializer);
        size_t      arity() const override;
        Object      call(Interpreter& interpreter, std::vector<Object>& arguments) override;
        std::string toString() const override;
        std::shared_ptr<LoxFunction> bind(std::shared_ptr<CppLoxInstance> instance);
    private:
        Function* m_declaration;
        // must be a ptr to original env , NOT a copy of the original
        // cause when the closure is copied from original, may be var declaration in this env is not completed
        std::shared_ptr<Environment> m_closure;
        bool m_isInitializer;
};

#endif // LOXFUNCTION_H