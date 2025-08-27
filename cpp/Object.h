#ifndef OBJECT_H
#define OBJECT_H
#include <variant>
#include <string>
#include <memory>
class CppLoxCallable;
class CppLoxClass;
class CppLoxInstance;
// using Object = std::variant<nullptr_t, double, bool, std::string, CppLoxCallable*>;
using Object = std::variant<nullptr_t, double, bool, std::string, 
                            std::shared_ptr<CppLoxCallable>, std::shared_ptr<CppLoxInstance>>;

#endif // OBJECT_H