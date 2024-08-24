#pragma once

#include "src/semantics/object/lox_callable.h"
#include "src/syntactics/stmt.h"
#include <memory>

struct LoxFunction final : LoxCallable {
    LoxFunction(Token name, std::vector<Token> params,
                std::vector<std::shared_ptr<Stmt>> body, Environment *closure);
    LoxFunction(const Stmt::Function &declaration, Environment *closure);
    LoxFunction(const Expr::Lambda &declaration, Environment *closure);

    std::string to_string() const override;
    LoxObject call(AbstractInterpreter &interpreter,
                   const std::vector<LoxObject> &arguments) override;
    size_t arity() const;

  private:
    // If named, this is the name of the function. Otherwise, it is the keyword
    // "fun".
    Token identifier;
    std::vector<Token> params;
    std::vector<std::shared_ptr<Stmt>> body;
    Environment *closure;
};
