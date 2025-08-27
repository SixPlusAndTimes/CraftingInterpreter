#ifndef CPPLOXINSTANCE_H
#define CPPLOXINSTANCE_H
#include "CppLoxClass.h"
#include "Token.h"
#include <unordered_map>
class CppLoxInstance {
    public:
                        CppLoxInstance(CppLoxClass* klass);
        std::string     toString() const;
        Object          get(std::shared_ptr<Token> name);      
        void            set(std::shared_ptr<Token> name, const Object& value);      

    private:
        CppLoxClass&    m_klass;
        std::unordered_map<std::string, Object> m_fields;
};

#endif // CPPLOXINSTANCE_H