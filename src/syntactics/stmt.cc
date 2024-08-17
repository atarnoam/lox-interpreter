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
