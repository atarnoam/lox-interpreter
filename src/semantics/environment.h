#pragma once

#include "src/semantics/lox_object.h"
#include <string>
#include <unordered_map>

struct Environment {
    using VarName = std::string;

    Environment();

    void define(const std::string &name, const LoxObject &value);
    void assign(const Token &name, const LoxObject &value);

    LoxObject &get(const Token &name);

  private:
    std::unordered_map<VarName, LoxObject> values;
};