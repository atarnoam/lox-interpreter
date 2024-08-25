#include "lox_class.h"

#include "src/semantics/object/lox_instance.h"

LoxClass::LoxClass(std::string name, std::shared_ptr<LoxClass> superclass,
                   LoxClass::MethodMap methods)
    : name(std::move(name)), superclass(std::move(superclass)),
      methods(std::move(methods)) {}

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

    auto initializer = find_method("init");
    if (initializer) {
        initializer->bind({"this", instance}, interpreter.environments)
            ->call(interpreter, arguments);
    }

    return instance;
}

size_t LoxClass::arity() const {
    auto initializer = find_method("init");
    if (initializer) {
        return initializer->arity();
    } else {
        return 0;
    }
}
