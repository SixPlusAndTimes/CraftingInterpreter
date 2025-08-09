#ifndef OBJECT_H
#define OBJECT_H
#include <variant>
#include <string>
#include <memory>
class CppLoxCallable;
using Object = std::variant<nullptr_t, double, bool, std::string, CppLoxCallable*>;

#endif // OBJECT_H