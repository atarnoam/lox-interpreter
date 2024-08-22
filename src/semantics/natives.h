#pragma once

#include "src/semantics/lox_callable.h"
#include <memory>
#include <string>
#include <utility>

struct ClockFun final : LoxCallable {
    virtual std::string to_string() const override;
    LoxObject call(AbstractInterpreter &interpreter,
                   const std::vector<LoxObject> &arguments) override;

    int arity() const override;
};

extern const std::vector<std::pair<std::string, LoxObject>> natives;
