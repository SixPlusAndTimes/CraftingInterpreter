#include "LoxFunction.h"
#include "Environment.h"
#include "Interpreter.h"
#include "utils.h"
#include "RuntimeError.h"
#include "Environment.h"

LoxFunction::LoxFunction(Function* declaration, const Environment& env) {
    m_declaration = declaration;
    m_closure = env;
    std::cout << "      capture env, copyfrom envptr = " << &env << " this ptr = " << &m_closure << std::endl;
}


Object LoxFunction::call(Interpreter& interpreter, std::vector<Object>& arguments) {
    LOG_DEBUG("Function call begin functionname = {}, argumentsize = {}", m_declaration->m_name->m_lexeme, arguments.size());
    // create local env when the funcion is called.
    // This feature enable Lox to do recursion calls
    std::unique_ptr<Environment> env = std::make_unique<Environment>(&m_closure, interpreter.m_globalEnvironment.get());

    for (size_t i = 0; i < m_declaration->m_params->size(); ++i) {
      // should we consider the case that function is a argument?
        if (std::holds_alternative<CppLoxCallable*>(arguments[i])) {
            LOG_DEBUG("Define function, name:[{}] in new env", m_declaration->m_name->m_lexeme);
            CppLoxCallable* func = std::get<CppLoxCallable*>(arguments[i]);
            LoxFunction* loxFuncPtr = static_cast<LoxFunction*>(func);
            env->define(m_declaration->m_params->at(i)->m_lexeme, std::make_shared<LoxFunction>(loxFuncPtr->m_declaration, loxFuncPtr->m_closure));
        }else {
            LOG_DEBUG("Define var, name:[{}] in new env", m_declaration->m_name->m_lexeme);
            env->define(m_declaration->m_params->at(i)->m_lexeme, arguments[i]);
        }
    }
    try {
            interpreter.executeBlock(m_declaration->m_body, env.get());
    } catch (ReturnException& returnExcept){
            LOG_DEBUG("catch returnexcept");
            return returnExcept.m_value;
    }

    return nullptr;
}

size_t LoxFunction::arity() const {
    return m_declaration->m_params->size();
}

std::string LoxFunction::toString() const{
    return "<fn " + m_declaration->m_name->m_lexeme + ">";
}
