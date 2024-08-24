#include "lox_function.h"

#include "src/semantics/abstract_interpreter.h"
#include "src/semantics/environment.h"
#include "src/semantics/object/lox_object.h"
#include "src/semantics/return.h"

LoxFunction::LoxFunction(std::shared_ptr<Stmt::Function> declaration,
                         Environment *closure)
    : declaration(std::move(declaration)), closure(closure) {}

LoxFunction::LoxFunction(const Stmt::Function &declaration,
                         Environment *closure)
    : LoxFunction(std::make_shared<Stmt::Function>(declaration), closure) {}

std::string LoxFunction::to_string() const {
    return "<fun " + declaration->name.lexeme + ">";
}

LoxObject LoxFunction::call(AbstractInterpreter &interpreter,
                            const std::vector<LoxObject> &arguments) {
    Environment *func_environment =
        interpreter.environments.add_environment(closure);
    for (size_t i = 0; i < declaration->params.size(); ++i) {
        func_environment->define(declaration->params[i].lexeme, arguments[i]);
    }

    Environment *previous_environment = interpreter.curr_environment;
    try {
        interpreter.execute_block(declaration->body, func_environment);
    } catch (const Return &return_value) {
        interpreter.curr_environment = previous_environment;
        return return_value.return_value;
    }
    return LoxObject{};
}

int LoxFunction::arity() const { return declaration->params.size(); }
