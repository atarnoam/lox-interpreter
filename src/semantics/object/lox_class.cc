#include "lox_class.h"

LoxClass::LoxClass(std::string name) : name(std::move(name)) {}

std::string LoxClass::to_string() const { return name; }

LoxObject LoxClass::call(AbstractInterpreter &interpreter,
                         const std::vector<LoxObject> &arguments) {
    return LoxObject();
}

size_t LoxClass::arity() const { return 0; }
