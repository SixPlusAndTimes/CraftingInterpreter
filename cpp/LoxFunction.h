#ifndef LOXFUNCTION_H
#define LOXFUNCTION_H
#include "CppLoxCallable.h"
#include "Stmt.h"

class LoxFunction : public CppLoxCallable {
    public:
                    LoxFunction(const Function& declaration);
        size_t      arity() const override;
        Object      call(Interpreter& interpreter, std::vector<Object>& arguments) override;
        std::string toString() const override;
    private:
        const Function* m_declaration;
        Environment8    m_closure.
};

#endif // LOXFUNCTION_H