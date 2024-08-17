#include "src/syntactics/expr.h"

Expr::Binary::Binary(std::unique_ptr<Expr> left, Token op,
                     std::unique_ptr<Expr> right)
    : left(std::move(left)), op(std::move(op)), right(std::move(right)) {}

void Expr::Binary::accept(ExprVisitor &visitor) const {
    visitor.visit_binary_expr(*this);
}

Expr::Grouping::Grouping(std::unique_ptr<Expr> expression)
    : expression(std::move(expression)) {}

void Expr::Grouping::accept(ExprVisitor &visitor) const {
    visitor.visit_grouping_expr(*this);
}

Expr::Literal::Literal(Token value) : value(std::move(value)) {}

void Expr::Literal::accept(ExprVisitor &visitor) const {
    visitor.visit_literal_expr(*this);
}

Expr::Unary::Unary(Token op, std::unique_ptr<Expr> right)
    : op(std::move(op)), right(std::move(right)) {}

void Expr::Unary::accept(ExprVisitor &visitor) const {
    visitor.visit_unary_expr(*this);
}
