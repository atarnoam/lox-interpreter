#include "abstract_interpreter.h"

AbstractInterpreter::AbstractInterpreter()
    : globals(std::make_shared<Environment>()), curr_environment(globals) {}

void AbstractInterpreter::execute(const std::shared_ptr<Stmt> &stmt) {
    return execute(stmt.get());
}

void AbstractInterpreter::execute_block(
    const std::vector<std::shared_ptr<Stmt>> &stmts,
    const std::shared_ptr<Environment> &environment) {
    auto previous_environment = curr_environment;
    curr_environment = environment;
    for (const auto &stmt : stmts) {
        execute(stmt);
    }
    curr_environment = previous_environment;
}
