#pragma once

#include "src/semantics/object/lox_callable.h"
#include "src/syntactics/stmt.h"
#include <memory>

struct LoxFunction final : LoxCallable {
    LoxFunction(std::shared_ptr<Stmt::Function> declaration,
                std::shared_ptr<Environment> closure);
    LoxFunction(const Stmt::Function &declaration,
                std::shared_ptr<Environment> closure);

    std::string to_string() const override;
    LoxObject call(AbstractInterpreter &interpreter,
                   const std::vector<LoxObject> &arguments) override;
    int arity() const;

    std::shared_ptr<Stmt::Function> declaration;
    std::shared_ptr<Environment> closure;
};
