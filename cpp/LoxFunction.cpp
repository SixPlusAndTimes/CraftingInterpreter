#include "LoxFunction.h"
#include "Environment.h"
#include "Interpreter.h"

LoxFunction::LoxFunction(const Function& declaration) {
    m_declaration = &declaration;
}


Object LoxFunction::call(Interpreter& interpreter, const std::vector<Object>& arguments) const {
    // create local env when the funcion is called.
    // This feature enable Lox to do recursion calls
    std::unique_ptr<Environment> env = std::make_unique<Environment>(interpreter.m_globalEnvironment.get());

    for (size_t i = 0; i < m_declaration->m_params->size(); ++i) {
      // should we consider the case that function is a argument?
      env->define(m_declaration->m_params->at(i)->m_lexeme, arguments[i]);
    }

    interpreter.executeBlock(m_declaration->m_body, env.get());

    return nullptr;
}

size_t LoxFunction::arity() const {
    return m_declaration->m_params->size();
}

std::string LoxFunction::toString() const{
    return "<fn " + m_declaration->m_name->m_lexeme + ">";
}
