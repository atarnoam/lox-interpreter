#pragma once

#include "src/semantics/abstract_interpreter.h"
#include "src/semantics/object/lox_callable.fwd.h"

#include <memory>
#include <vector>

struct LoxObject;

struct LoxCallable {
    virtual ~LoxCallable() = default;

    virtual std::string to_string() const = 0;
    virtual LoxObject call(AbstractInterpreter &interpreter,
                           const std::vector<LoxObject> &arguments) = 0;
    virtual int arity() const = 0;
};
