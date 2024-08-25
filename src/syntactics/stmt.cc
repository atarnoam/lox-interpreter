#include "src/syntactics/stmt.h"

Stmt::Block::Block(std::vector<std::shared_ptr<Stmt>> statements)
    : statements(std::move(statements)) {}

void Stmt::Block::accept(StmtVisitor &visitor) const {
    visitor.visit_block_stmt(*this);
}

Stmt::Class::Class(Token name, std::shared_ptr<Expr::Variable> superclass,
                   std::vector<std::shared_ptr<Stmt::Function>> methods)
    : name(std::move(name)), superclass(std::move(superclass)),
      methods(std::move(methods)) {}

void Stmt::Class::accept(StmtVisitor &visitor) const {
    visitor.visit_class_stmt(*this);
}

Stmt::Expression::Expression(std::shared_ptr<Expr> expression)
    : expression(std::move(expression)) {}

void Stmt::Expression::accept(StmtVisitor &visitor) const {
    visitor.visit_expression_stmt(*this);
}

Stmt::If::If(std::shared_ptr<Expr> condition, std::shared_ptr<Stmt> then_branch,
             std::shared_ptr<Stmt> else_branch)
    : condition(std::move(condition)), then_branch(std::move(then_branch)),
      else_branch(std::move(else_branch)) {}

void Stmt::If::accept(StmtVisitor &visitor) const {
    visitor.visit_if_stmt(*this);
}

Stmt::Function::Function(Token name, std::vector<Token> params,
                         std::vector<std::shared_ptr<Stmt>> body)
    : name(std::move(name)), params(std::move(params)), body(std::move(body)) {}

void Stmt::Function::accept(StmtVisitor &visitor) const {
    visitor.visit_function_stmt(*this);
}

Stmt::Print::Print(std::shared_ptr<Expr> expression)
    : expression(std::move(expression)) {}

void Stmt::Print::accept(StmtVisitor &visitor) const {
    visitor.visit_print_stmt(*this);
}

Stmt::Return::Return(Token keyword, std::shared_ptr<Expr> value)
    : keyword(std::move(keyword)), value(std::move(value)) {}

void Stmt::Return::accept(StmtVisitor &visitor) const {
    visitor.visit_return_stmt(*this);
}

Stmt::Var::Var(Token name, std::shared_ptr<Expr> initializer)
    : name(std::move(name)), initializer(std::move(initializer)) {}

void Stmt::Var::accept(StmtVisitor &visitor) const {
    visitor.visit_var_stmt(*this);
}

Stmt::While::While(std::shared_ptr<Expr> condition, std::shared_ptr<Stmt> body)
    : condition(std::move(condition)), body(std::move(body)) {}

void Stmt::While::accept(StmtVisitor &visitor) const {
    visitor.visit_while_stmt(*this);
}
