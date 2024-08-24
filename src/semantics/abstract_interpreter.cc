#include "abstract_interpreter.h"

AbstractInterpreter::AbstractInterpreter()
    : environments(), curr_environment(&environments.globals()) {}

void AbstractInterpreter::execute(const std::shared_ptr<Stmt> &stmt) {
    return execute(stmt.get());
}

void AbstractInterpreter::execute_block(
    const std::vector<std::shared_ptr<Stmt>> &stmts, Environment *environment) {
    Environment *previous_environment = curr_environment;
    curr_environment = environment;
    for (const auto &stmt : stmts) {
        execute(stmt);
    }
    curr_environment = previous_environment;
}

Environment *AbstractInterpreter::globals() { return &environments.globals(); }
