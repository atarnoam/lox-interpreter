#include "src/syntactics/stmt.h"

Expression::Expression(std::unique_ptr<Expr> expression)
    : expression(std::move(expression)) {}

void Expression::accept(StmtVisitor &visitor) const {
    visitor.visit_expression(*this);
}

Print::Print(std::unique_ptr<Expr> expression)
    : expression(std::move(expression)) {}

void Print::accept(StmtVisitor &visitor) const { visitor.visit_print(*this); }
