#pragma once

#include "src/semantics/object/lox_object.h"

#include <memory>
#include <optional>
#include <string>
#include <unordered_map>

struct Environment {
    using VarName = std::string;

    Environment();
    explicit Environment(Environment *enclosing);

    void define(const VarName &name, const LoxObject &value);
    void assign(const Token &name, const LoxObject &value);

    LoxObject &get(const Token &name);

    Environment *enclosing;

    LoxObject &get_at(const Token &name, int distance);
    void assign_at(const Token &name, const LoxObject &value, int distance);

  private:
    Environment *ancestor(int distance);

    std::unordered_map<VarName, LoxObject> values;
};

struct EnvironmentTree {
    EnvironmentTree();

    Environment *add_environment(Environment *enclosing);

    Environment &globals();

  private:
    std::vector<std::unique_ptr<Environment>> environments;
    Environment *m_globals;
};