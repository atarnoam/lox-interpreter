#include "environment.h"
#include "src/semantics/environment.h"
#include "src/semantics/runtime_error.h"

Environment::Environment() : values() {}

void Environment::define(const std::string &name, const LoxObject &value) {
    values[name] = value;
}

void Environment::assign(const Token &name, const LoxObject &value) {
    if (!values.contains(name.lexeme)) {
        throw RuntimeError(name, "Undefined variable '" + name.lexeme + "'.");
    }
    values[name.lexeme] = value;
}

LoxObject &Environment::get(const Token &name) {
    if (!values.contains(name.lexeme)) {
        throw RuntimeError(name, "Undefined variable '" + name.lexeme + "'.");
    }
    return values[name.lexeme];
}
