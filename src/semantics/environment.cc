#include "environment.h"
#include "src/semantics/environment.h"
#include "src/semantics/runtime_error.h"

Environment::Environment(Environment *enclosing)
    : enclosing(enclosing), values() {}

Environment::Environment() : enclosing(nullptr), values() {}

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

LoxObject &Environment::get(const Token &name) {
    if (values.contains(name.lexeme)) {
        return values.at(name.lexeme);
    }

    if (enclosing != nullptr) {
        return enclosing->get(name);
    }
    throw RuntimeError(name, "Undefined variable '" + name.lexeme + "'.");
}

EnvironmentTree::EnvironmentTree() : environments(), m_globals(nullptr) {
    environments.emplace_back(std::make_unique<Environment>());
    m_globals = environments.front().get();
}

Environment *EnvironmentTree::add_environment(Environment *enclosing) {
    return environments.emplace_back(std::make_unique<Environment>(enclosing))
        .get();
}

Environment &EnvironmentTree::globals() { return *m_globals; }
