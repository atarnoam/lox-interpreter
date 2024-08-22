#include "src/semantics/lox_function.h"

#include "lox_function.h"
#include "src/semantics/abstract_interpreter.h"
#include "src/semantics/environment.h"
#include "src/semantics/lox_object.h"

LoxFunction::LoxFunction(std::shared_ptr<Stmt::Function> declaration)
    : declaration(std::move(declaration)) {}

LoxFunction::LoxFunction(const Stmt::Function &declaration)
    : LoxFunction(std::make_shared<Stmt::Function>(declaration)) {}

std::string LoxFunction::to_string() const {
    return "<fun " + declaration->name.lexeme + ">";
}

LoxObject LoxFunction::call(AbstractInterpreter &interpreter,
                            const std::vector<LoxObject> &arguments) {
    auto environment = std::make_shared<Environment>(*interpreter.globals);
    for (size_t i = 0; i < declaration->params.size(); ++i) {
        environment->define(declaration->params[i].lexeme, arguments[i]);
    }
    interpreter.execute_block(declaration->body, environment);
    return LoxObject();
}

int LoxFunction::arity() const { return declaration->params.size(); }
