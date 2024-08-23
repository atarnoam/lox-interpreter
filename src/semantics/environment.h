#pragma once

#include "src/semantics/object/lox_object.h"
#include <optional>
#include <string>
#include <unordered_map>

struct Environment {
    using VarName = std::string;

    Environment();
    Environment(const Environment &) = default;
    Environment(std::shared_ptr<Environment> enclosing);

    void define(const std::string &name, const LoxObject &value);
    void assign(const Token &name, const LoxObject &value);

    LoxObject &get(const Token &name);
    const LoxObject &get(const Token &name) const;

  private:
    std::shared_ptr<Environment> enclosing;
    std::unordered_map<VarName, LoxObject> values;
};