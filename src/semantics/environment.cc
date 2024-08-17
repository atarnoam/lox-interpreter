#include "environment.h"
#include "src/semantics/environment.h"
#include "src/semantics/runtime_error.h"

Environment::Environment() : enclosing(nullptr), values() {}

Environment::Environment(std::shared_ptr<Environment> enclosing)
    : enclosing(std::move(enclosing)), values() {}

void Environment::define(const std::string &name, const LoxObject &value) {
    values[name] = value;
}

void Environment::assign(const Token &name, const LoxObject &value) {
    if (values.contains(name.lexeme)) {
        values[name.lexeme] = value;
        return;
    }

    if (enclosing) {
        enclosing->assign(name, value);
        return;
    }

    throw RuntimeError(name, "Undefined variable '" + name.lexeme + "'.");
}

const LoxObject &Environment::get(const Token &name) const {
    if (values.contains(name.lexeme)) {
        return values.at(name.lexeme);
    }

    if (enclosing) {
        return enclosing->get(name);
    }
    throw RuntimeError(name, "Undefined variable '" + name.lexeme + "'.");
}

LoxObject &Environment::get(const Token &name) {
    return const_cast<LoxObject &>(
        const_cast<const Environment *>(this)->get(name));
}