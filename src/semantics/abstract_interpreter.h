#pragma once

#include "src/semantics/environment.h"
#include "src/syntactics/stmt.h"

#include <memory>
#include <unordered_map>

struct AbstractInterpreter {
    AbstractInterpreter();
    virtual ~AbstractInterpreter() = default;

    virtual void execute(const Stmt *stmt) = 0;
    virtual void execute(const std::shared_ptr<Stmt> &stmt);

    virtual void execute_block(const std::vector<std::shared_ptr<Stmt>> &stmts,
                               Environment *environment);

    friend struct LoxFunction;
    friend struct Resolver;

    EnvironmentTree environments;

  protected:
    virtual void resolve(const Expr *, int depth);

    Environment *globals();
    Environment *curr_environment;
    std::unordered_map<const Expr *, int> locals;
};