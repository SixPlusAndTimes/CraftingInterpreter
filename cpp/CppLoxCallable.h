#ifndef CPPLOXCALLABLE_H
#define CPPLOXCALLABLE_H
#include <vector>
#include <string>
#include "Object.h"
class Interpreter;


class CppLoxCallable {
    public:
        virtual ~CppLoxCallable() {}
        virtual size_t arity() const = 0;
        virtual std::string toString() const = 0 ;
        virtual Object call(Interpreter& interpreter, std::vector<Object>& arguments) = 0;
};

#endif // CPPLOXCALLABLE_H