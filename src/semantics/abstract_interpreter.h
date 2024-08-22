#pragma once

#include "src/semantics/environment.h"
#include "src/syntactics/stmt.h"

#include <memory>

struct AbstractInterpreter {
    AbstractInterpreter();
    virtual ~AbstractInterpreter() = default;

    virtual void execute(const Stmt *stmt) = 0;
    virtual void execute(const std::shared_ptr<Stmt> &stmt);

    virtual void execute_block(const std::vector<std::shared_ptr<Stmt>> &stmts,
                               const std::shared_ptr<Environment> &environment);

    friend class LoxFunction;

    std::shared_ptr<Environment> globals;

  protected:
    std::shared_ptr<Environment> curr_environment;
};