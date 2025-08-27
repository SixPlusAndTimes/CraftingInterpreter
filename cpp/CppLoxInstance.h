#ifndef CPPLOXINSTANCE_H
#define CPPLOXINSTANCE_H
#include "CppLoxClass.h"

class CppLoxInstance {
    public:
                        CppLoxInstance(CppLoxClass* klass);
        std::string     toString() const;

    private:
        CppLoxClass&    m_klass;
};

#endif // CPPLOXINSTANCE_H