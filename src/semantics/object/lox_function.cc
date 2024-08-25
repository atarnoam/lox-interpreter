#include "lox_function.h"

#include "src/semantics/abstract_interpreter.h"
#include "src/semantics/environment.h"
#include "src/semantics/object/lox_object.h"
#include "src/semantics/return.h"

LoxFunction::LoxFunction(Token name, std::vector<Token> params,
                         std::vector<std::shared_ptr<Stmt>> body,
                         Environment *closure)
    : identifier(std::move(name)), params(std::move(params)),
      body(std::move(body)), closure(closure) {}

LoxFunction::LoxFunction(const Stmt::Function &declaration,
                         Environment *closure)
    : LoxFunction(declaration.name, declaration.params, declaration.body,
                  closure) {}

LoxFunction::LoxFunction(const Expr::Lambda &declaration, Environment *closure)
    : LoxFunction(declaration.keyword, declaration.params, declaration.body,
                  closure) {}

std::string LoxFunction::to_string() const {
    if (identifier.type == IDENTIFIER) {
        return "<fun " + identifier.lexeme + ">";
    } else {
        return "<anonymous function>";
    }
}

LoxObject LoxFunction::call(AbstractInterpreter &interpreter,
                            const std::vector<LoxObject> &arguments) {
    Environment *func_environment =
        interpreter.environments.add_environment(closure);
    for (size_t i = 0; i < params.size(); ++i) {
        func_environment->define(params[i].lexeme, arguments[i]);
    }

    Environment *previous_environment = interpreter.curr_environment;
    try {
        interpreter.execute_block(body, func_environment);
    } catch (const Return &return_value) {
        interpreter.curr_environment = previous_environment;
        return return_value.return_value;
    }
    return LoxObject{};
}

size_t LoxFunction::arity() const { return params.size(); }

std::shared_ptr<LoxFunction>
LoxFunction::bind(const std::pair<std::string, LoxObject> &to_bind,
                  EnvironmentTree &envs) {
    Environment *new_enviroment = envs.add_environment(closure);
    auto [name, object] = to_bind;
    new_enviroment->define(name, object);
    return std::make_shared<LoxFunction>(identifier, params, body,
                                         new_enviroment);
}
