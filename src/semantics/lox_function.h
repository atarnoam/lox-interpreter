#pragma once

#include "src/semantics/lox_callable.h"
#include "src/syntactics/stmt.h"
#include <memory>

struct LoxFunction final : LoxCallable {
    LoxFunction(std::shared_ptr<Stmt::Function> declaration);
    LoxFunction(const Stmt::Function &declaration);

    std::string to_string() const override;
    LoxObject call(AbstractInterpreter &interpreter,
                   const std::vector<LoxObject> &arguments) override;
    int arity() const;

    std::shared_ptr<Stmt::Function> declaration;
};