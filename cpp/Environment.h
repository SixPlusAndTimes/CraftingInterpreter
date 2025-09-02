#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H
#include <unordered_map>
#include "Token.h"
class Environment{
public:
                                                Environment();
                                                Environment(std::shared_ptr<Environment> enclosing);
    virtual                                     ~Environment();
    void                                        define(const std::string& name, const Object& value);
    void                                        assign(const Token& name, const Object& value);
    void                                        assignAt(int distance, const Token& name, const Object& value);
    Object                                      get(Token name);
    Object                                      getAt(int distance, std::string name);
    std::unordered_map<std::string, Object>&    getValues();
    Environment&                                ancestor(int distance);
    std::shared_ptr<Environment>                getEnclosing();
private:
    std::unordered_map<std::string, Object>     m_values;   
    std::shared_ptr<Environment>                m_enclosing; 
};
#endif // ENVIRONMENT_H