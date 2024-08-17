#include "src/syntactics/stmt.h"

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
