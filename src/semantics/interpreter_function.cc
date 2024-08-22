#include "src/semantics/interpreter.h"

#include "src/semantics/lox_function.h"

#include <iostream>

void _visit_function_stmt(Interpreter &interpreter,
                          const Stmt::Function &func) {
    auto function = std::make_shared<LoxFunction>(func);
    interpreter.curr_environment->define(func.name.lexeme, function);
}
