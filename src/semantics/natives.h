#pragma once

#include "src/semantics/object/lox_callable.h"
#include <memory>
#include <string>
#include <utility>

extern const std::vector<std::pair<std::string, LoxObject>> natives;

struct ClockFun final : LoxCallable {
    std::string to_string() const override;
    LoxObject call(AbstractInterpreter &interpreter,
                   const std::vector<LoxObject> &arguments) override;

    int arity() const override;
};

struct ToStringFun final : LoxCallable {
    std::string to_string() const override;
    LoxObject call(AbstractInterpreter &interpreter,
                   const std::vector<LoxObject> &arguments) override;

    int arity() const override;
};