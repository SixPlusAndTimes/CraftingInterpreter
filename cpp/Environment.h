#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H
#include <unordered_map>
#include "Token.h"
class Environment{
public:
    std::unordered_map<std::string, Object> m_values;   
    void    define(std::string name, Object value);
    Object  get(Token name);
};
#endif // ENVIRONMENT_H