#include "lox_class.h"
#include "lox_instance.h"

LoxClass::LoxClass(std::string name) : name(std::move(name)) {}

std::string LoxClass::to_string() const { return name; }

LoxObject LoxClass::call(AbstractInterpreter &interpreter,
                         const std::vector<LoxObject> &arguments) {
    LoxObject instance(std::make_shared<LoxInstance>(this));
    return instance;
}

size_t LoxClass::arity() const { return 0; }
