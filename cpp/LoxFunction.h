#ifndef LOXFUNCTION_H
#define LOXFUNCTION_H
#include "CppLoxCallable.h"
#include "Stmt.h"

class LoxFunction : public CppLoxCallable {
    public:
                    LoxFunction(const Function& declaration);
        size_t      arity() const override;
        Object      call(Interpreter& interpreter, const std::vector<Object>& arguments) const override;
        std::string toString() const override;
    private:
        const Function* m_declaration;
};

#endif // LOXFUNCTION_H