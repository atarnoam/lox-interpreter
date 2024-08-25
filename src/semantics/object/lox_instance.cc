#include "lox_instance.h"

#include "src/semantics/runtime_error.h"

LoxInstance::LoxInstance(const LoxClass *lclass) : lclass(lclass), fields() {}

std::string LoxInstance::to_string() const {
    return lclass->to_string() + " instance";
}

LoxObject LoxInstance::get(Token name) const {
    // Fields shadow methods.
    if (fields.contains(name.lexeme)) {
        return fields.at(name.lexeme);
    }

    if (auto method = lclass->find_method(name.lexeme)) {
        return method;
    }

    throw RuntimeError(name, "Undefined property '" + name.lexeme + "'.");
}

void LoxInstance::set(Token name, const LoxObject &value) {
    fields[name.lexeme] = value;
}

std::ostream &operator<<(std::ostream &os, const LoxInstance &callable) {
    return os << callable.to_string();
}
