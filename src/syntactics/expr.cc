#include "src/syntactics/expr.h"

Expr::Assign::Assign(Token name, std::unique_ptr<Expr> value)
    : name(std::move(name)), value(std::move(value)) {}

void Expr::Assign::accept(ExprVisitor &visitor) const {
    visitor.visit_assign_expr(*this);
}

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

Expr::Variable::Variable(Token name) : name(std::move(name)) {}

void Expr::Variable::accept(ExprVisitor &visitor) const {
    visitor.visit_variable_expr(*this);
}
