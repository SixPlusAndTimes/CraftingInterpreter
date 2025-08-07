#ifndef CPPLOXCALLABLE_H
#define CPPLOXCALLABLE_H
#include <vector>
#include "Object.h"
class Interpreter;


class CppLoxCallable {
    public:
        virtual size_t arity();
        virtual Object call(Interpreter& interpreter, std::vector<Object> arguments);
};

#endif // CPPLOXCALLABLE_H