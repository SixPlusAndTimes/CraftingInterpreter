#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H
#include <unordered_map>
#include "Token.h"
class Environment{
public:
    Environment();
    Environment(Environment* enclosing);
    virtual ~Environment();
    void    define(const std::string& name, const Object& value);
    void    assign(const Token& name, const Object& value);
    Object  get(Token name);
private:
    std::unordered_map<std::string, Object>         m_values;   
    Environment* m_enclosing; 
};
#endif // ENVIRONMENT_H