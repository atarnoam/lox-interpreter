#include "lox_class.h"

#include "src/semantics/object/lox_instance.h"

LoxClass::LoxClass(std::string name, LoxClass::MethodMap methods)
    : name(std::move(name)), methods(std::move(methods)) {}

std::shared_ptr<LoxFunction>
LoxClass::find_method(const std::string &name) const {
    if (!methods.contains(name)) {
        return nullptr;
    }
    return methods.at(name);
}

std::string LoxClass::to_string() const { return name; }

LoxObject LoxClass::call(AbstractInterpreter &interpreter,
                         const std::vector<LoxObject> &arguments) {
    LoxObject instance(std::make_shared<LoxInstance>(this));
    return instance;
}

size_t LoxClass::arity() const { return 0; }
