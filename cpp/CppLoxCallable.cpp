#include "CppLoxCallable.h"
#include <iostream>
const static size_t FUNCTION_ARGUMENT_LIMIT = 255;
size_t arity() {
    std::cout << "calling base arity(), not implemented\n" ;
    return FUNCTION_ARGUMENT_LIMIT + 1;
}

Object call(Interpreter& interpreter, std::vector<Object> arguments) {

    std::cout << "calling base call(), not implemented\n" ;
    return nullptr;
}
