#ifndef LOXFUNCTION_H
#define LOXFUNCTION_H
#include "CppLoxCallable.h"
#include "Stmt.h"
#include "Environment.h"
class Environment;
class LoxFunction : public CppLoxCallable {
    public:
                    LoxFunction(Function* declaration, Environment& env);
        size_t      arity() const override;
        Object      call(Interpreter& interpreter, std::vector<Object>& arguments) override;
        std::string toString() const override;
    private:
        Function* m_declaration;
        // must be a ptr to original env , NOT a copy of the original
        // cause when the closure is copied from original, may be var declaration int the env is not completed
        Environment*    m_closure;
};

#endif // LOXFUNCTION_H