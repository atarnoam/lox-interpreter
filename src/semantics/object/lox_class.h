#pragma once

#include "lox_class.fwd.h"

#include "src/semantics/object/lox_callable.h"
#include "src/semantics/object/lox_function.h"
#include "src/semantics/object/lox_instance.fwd.h"

#include <memory>
#include <optional>
#include <string>
#include <unordered_map>

struct LoxClass final : LoxCallable {
    using MethodMap =
        std::unordered_map<std::string, std::shared_ptr<LoxFunction>>;

    LoxClass(std::string name, std::shared_ptr<LoxClass> superclass,
             MethodMap methods);

    std::shared_ptr<LoxFunction> find_method(const std::string &name) const;

    std::string to_string() const override;

    LoxObject call(AbstractInterpreter &interpreter,
                   const std::vector<LoxObject> &arguments) override;

    size_t arity() const;

  private:
    std::string name;
    std::shared_ptr<LoxClass> superclass;
    MethodMap methods;
};