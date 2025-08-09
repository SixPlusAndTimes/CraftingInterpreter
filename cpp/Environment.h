#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H
#include <unordered_map>
#include "Token.h"
class Environment{
public:
    Environment();
    Environment(Environment* enclosing);
    void    define(const std::string& name, const Object& value);
    void    define(const std::string& name, std::unique_ptr<CppLoxCallable> function);
    void    assign(const Token& name, const Object& value);
    Object  get(Token name);
private:
    std::unordered_map<std::string, Object>         m_values;   
    std::vector <std::unique_ptr<CppLoxCallable>>   m_functions; // just for the convenience of managing function object
    Environment* m_enclosing; 
    
};
#endif // ENVIRONMENT_H