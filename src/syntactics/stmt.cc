#include "src/syntactics/stmt.h"

Stmt::Block::Block(std::vector<std::shared_ptr<Stmt>> statements)
    : statements(std::move(statements)) {}

void Stmt::Block::accept(StmtVisitor &visitor) const {
    visitor.visit_block_stmt(*this);
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

Stmt::Print::Print(std::shared_ptr<Expr> expression)
    : expression(std::move(expression)) {}

void Stmt::Print::accept(StmtVisitor &visitor) const {
    visitor.visit_print_stmt(*this);
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
