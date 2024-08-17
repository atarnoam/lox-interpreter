#include "src/syntactics/stmt.h"

Stmt::Block::Block(std::vector<std::unique_ptr<Stmt>> statements)
    : statements(std::move(statements)) {}

void Stmt::Block::accept(StmtVisitor &visitor) const {
    visitor.visit_block_stmt(*this);
}

Stmt::Expression::Expression(std::unique_ptr<Expr> expression)
    : expression(std::move(expression)) {}

void Stmt::Expression::accept(StmtVisitor &visitor) const {
    visitor.visit_expression_stmt(*this);
}

Stmt::If::If(std::unique_ptr<Expr> condition, std::unique_ptr<Stmt> then_branch,
             std::unique_ptr<Stmt> else_branch)
    : condition(std::move(condition)), then_branch(std::move(then_branch)),
      else_branch(std::move(else_branch)) {}

void Stmt::If::accept(StmtVisitor &visitor) const {
    visitor.visit_if_stmt(*this);
}

Stmt::Print::Print(std::unique_ptr<Expr> expression)
    : expression(std::move(expression)) {}

void Stmt::Print::accept(StmtVisitor &visitor) const {
    visitor.visit_print_stmt(*this);
}

Stmt::Var::Var(Token name, std::unique_ptr<Expr> initializer)
    : name(std::move(name)), initializer(std::move(initializer)) {}

void Stmt::Var::accept(StmtVisitor &visitor) const {
    visitor.visit_var_stmt(*this);
}
