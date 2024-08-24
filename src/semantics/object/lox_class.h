#pragma once

#include "src/semantics/object/lox_callable.h"

#include <string>

struct LoxClass final : LoxCallable {
    LoxClass(std::string name);

    std::string to_string() const override;
    LoxObject call(AbstractInterpreter &interpreter,
                   const std::vector<LoxObject> &arguments) override;
    size_t arity() const;

  private:
    std::string name;
};