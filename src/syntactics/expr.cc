#include "src/syntactics/expr.h"

Binary::Binary(std::unique_ptr<Expr> left, Token op,
               std::unique_ptr<Expr> right)
    : left(std::move(left)), op(std::move(op)), right(std::move(right)) {}

void Binary::accept(ExprVisitor &visitor) const { visitor.visit_binary(*this); }

Grouping::Grouping(std::unique_ptr<Expr> expression)
    : expression(std::move(expression)) {}

void Grouping::accept(ExprVisitor &visitor) const {
    visitor.visit_grouping(*this);
}

Literal::Literal(Token value) : value(std::move(value)) {}

void Literal::accept(ExprVisitor &visitor) const {
    visitor.visit_literal(*this);
}

Unary::Unary(Token op, std::unique_ptr<Expr> right)
    : op(std::move(op)), right(std::move(right)) {}

void Unary::accept(ExprVisitor &visitor) const { visitor.visit_unary(*this); }
