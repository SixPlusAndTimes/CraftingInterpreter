#include <memory>
#include "cpplox.h"
bool cpplox::hadError = false;
bool cpplox::hadRuntimeError = false;

std::shared_ptr<Interpreter> cpplox::interpreterPtr = std::make_shared<Interpreter>();
