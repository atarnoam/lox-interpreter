#include "resolver.h"

#include "src/logging.h"

Resolver::Resolver(AbstractInterpreter &interpreter)
    : interpreter(interpreter), scopes(), current_function(FunctionType::NONE),
      current_class(ClassType::NONE), m_had_error(false) {}

void Resolver::resolve(const std::shared_ptr<Expr> &expr) {
    if (expr) {
        expr->accept(*this);
    }
}

void Resolver::resolve(const std::shared_ptr<Stmt> &stmt) {
    if (stmt) {
        stmt->accept(*this);
    }
}

void Resolver::resolve(const std::vector<std::shared_ptr<Stmt>> &stmts) {
    for (const auto &stmt : stmts) {
        resolve(stmt);
    }
}

bool Resolver::had_error() const { return m_had_error; }

void Resolver::visit_assign_expr(const Expr::Assign &expr) {
    resolve(expr.value);
    resolve_local(expr, expr.name);
}

void Resolver::visit_binary_expr(const Expr::Binary &expr) {
    resolve(expr.left);
    resolve(expr.right);
}

void Resolver::visit_call_expr(const Expr::Call &expr) {
    resolve(expr.callee);
    for (const auto &argument : expr.arguments) {
        resolve(argument);
    }
}

void Resolver::visit_get_expr(const Expr::Get &expr) { resolve(expr.object); }

void Resolver::visit_grouping_expr(const Expr::Grouping &expr) {
    resolve(expr.expression);
}

void Resolver::visit_lambda_expr(const Expr::Lambda &expr) {
    resolve_function(expr.params, expr.body, FunctionType::FUNCTION);
}

void Resolver::visit_literal_expr(const Expr::Literal &expr) { return; }

void Resolver::visit_logical_expr(const Expr::Logical &expr) {
    resolve(expr.left);
    resolve(expr.right);
}

void Resolver::visit_set_expr(const Expr::Set &expr) {
    resolve(expr.value);
    resolve(expr.object);
}

void Resolver::visit_this_expr(const Expr::This &expr) {
    if (current_class == ClassType::NONE) {
        report_resolve_error(expr.keyword,
                             "Can't use 'this' outside of a class.");
        return;
    }
    resolve_local(expr, expr.keyword);
}

void Resolver::visit_unary_expr(const Expr::Unary &expr) {
    resolve(expr.right);
}

void Resolver::visit_variable_expr(const Expr::Variable &var) {
    if (!scopes.empty() and scopes.back().check_scope(var.name.lexeme) ==
                                VariableStatus::DECLARED) {
        report_resolve_error(
            var.name, "Can't read local variable in its own initializer.");
        return;
    }
    resolve_local(var, var.name);
}

void Resolver::visit_block_stmt(const Stmt::Block &block) {
    begin_scope();
    resolve(block.statements);
    end_scope();
}

void Resolver::visit_expression_stmt(const Stmt::Expression &stmt) {
    resolve(stmt.expression);
}

void Resolver::visit_class_stmt(const Stmt::Class &stmt) {
    ClassType enclosing_class = current_class;
    current_class = ClassType::CLASS;

    declare(stmt.name);
    define(stmt.name);

    begin_scope();
    scopes.back().define("this");

    for (const auto &method : stmt.methods) {
        resolve_function(*method, FunctionType::METHOD);
    }

    end_scope();

    current_class = enclosing_class;
}

void Resolver::visit_if_stmt(const Stmt::If &stmt) {
    resolve(stmt.condition);
    resolve(stmt.then_branch);
    resolve(stmt.else_branch);
}

void Resolver::visit_function_stmt(const Stmt::Function &stmt) {
    declare(stmt.name);
    define(stmt.name);

    resolve_function(stmt, FunctionType::FUNCTION);
}

void Resolver::visit_print_stmt(const Stmt::Print &stmt) {
    resolve(stmt.expression);
}

void Resolver::visit_return_stmt(const Stmt::Return &stmt) {
    if (current_function == FunctionType::NONE) {
        report_resolve_error(stmt.keyword, "Can't return from top-level code.");
    }

    resolve(stmt.value);
}

void Resolver::visit_var_stmt(const Stmt::Var &stmt) {
    declare(stmt.name);
    if (stmt.initializer) {
        resolve(stmt.initializer);
    }
    define(stmt.name);
}

void Resolver::visit_while_stmt(const Stmt::While &stmt) {
    resolve(stmt.condition);
    resolve(stmt.body);
}

void Resolver::begin_scope() { scopes.emplace_back(); }

void Resolver::end_scope() { scopes.pop_back(); }

void Resolver::declare(const Token &var) {
    if (scopes.empty()) {
        return;
    }

    Scope &scope = scopes.back();
    if (scope.in_scope(var.lexeme)) {
        report_resolve_error(
            var, "Already a variable with this name in this scope.");
    }

    scope.declare(var.lexeme);
}

void Resolver::define(const Token &var) {
    if (scopes.empty()) {
        return;
    }

    Scope &scope = scopes.back();
    scope.define(var.lexeme);
}

void Resolver::resolve_local(const Expr &expr, const Token &token) {
    for (auto scope_it = scopes.rbegin(); scope_it != scopes.rend();
         ++scope_it) {
        if (scope_it->in_scope(token.lexeme)) {
            interpreter.resolve(&expr,
                                static_cast<int>(scopes.rbegin() - scope_it));
        }
    }
}

void Resolver::resolve_function(const Stmt::Function &function,
                                FunctionType type) {
    resolve_function(function.params, function.body, type);
}

void Resolver::resolve_function(const std::vector<Token> &params,
                                const std::vector<std::shared_ptr<Stmt>> &body,
                                FunctionType type) {
    FunctionType enclosing_function = current_function;
    current_function = type;

    begin_scope();
    for (const Token &param : params) {
        declare(param);
        define(param);
    }
    resolve(body);
    end_scope();

    current_function = enclosing_function;
}

std::string Resolver::report_resolve_error(const Token &token,
                                           const std::string &message,
                                           bool warning) {
    if (!warning) {
        m_had_error = true;
    }
    return report_token_error(token, message);
}

void Scope::declare(const std::string &name) { map[name] = false; }

void Scope::define(const std::string &name) { map[name] = true; }

bool Scope::in_scope(const std::string &name) const {
    return map.contains(name);
}

VariableStatus Scope::check_scope(const std::string &name) const {
    if (!in_scope(name)) {
        return VariableStatus::UNKNOWN;
    }
    return map.at(name) ? VariableStatus::DEFINED : VariableStatus::DECLARED;
}
