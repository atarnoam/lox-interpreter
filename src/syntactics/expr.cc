#include "src/syntactics/expr.h"

Expr::Assign::Assign(Token name, std::shared_ptr<Expr> value)
    : name(std::move(name)), value(std::move(value)) {}

void Expr::Assign::accept(ExprVisitor &visitor) const {
    visitor.visit_assign_expr(*this);
}

Expr::Binary::Binary(std::shared_ptr<Expr> left, Token op,
                     std::shared_ptr<Expr> right)
    : left(std::move(left)), op(std::move(op)), right(std::move(right)) {}

void Expr::Binary::accept(ExprVisitor &visitor) const {
    visitor.visit_binary_expr(*this);
}

Expr::Call::Call(std::shared_ptr<Expr> callee, Token paren,
                 std::vector<std::shared_ptr<Expr>> arguments)
    : callee(std::move(callee)), paren(std::move(paren)),
      arguments(std::move(arguments)) {}

void Expr::Call::accept(ExprVisitor &visitor) const {
    visitor.visit_call_expr(*this);
}

Expr::Grouping::Grouping(std::shared_ptr<Expr> expression)
    : expression(std::move(expression)) {}

void Expr::Grouping::accept(ExprVisitor &visitor) const {
    visitor.visit_grouping_expr(*this);
}

Expr::Lambda::Lambda(Token keyword, std::vector<Token> params,
                     std::vector<std::shared_ptr<Stmt>> body)
    : keyword(std::move(keyword)), params(std::move(params)),
      body(std::move(body)) {}

void Expr::Lambda::accept(ExprVisitor &visitor) const {
    visitor.visit_lambda_expr(*this);
}

Expr::Literal::Literal(Token value) : value(std::move(value)) {}

void Expr::Literal::accept(ExprVisitor &visitor) const {
    visitor.visit_literal_expr(*this);
}

Expr::Logical::Logical(std::shared_ptr<Expr> left, Token op,
                       std::shared_ptr<Expr> right)
    : left(std::move(left)), op(std::move(op)), right(std::move(right)) {}

void Expr::Logical::accept(ExprVisitor &visitor) const {
    visitor.visit_logical_expr(*this);
}

Expr::Unary::Unary(Token op, std::shared_ptr<Expr> right)
    : op(std::move(op)), right(std::move(right)) {}

void Expr::Unary::accept(ExprVisitor &visitor) const {
    visitor.visit_unary_expr(*this);
}

Expr::Variable::Variable(Token name) : name(std::move(name)) {}

void Expr::Variable::accept(ExprVisitor &visitor) const {
    visitor.visit_variable_expr(*this);
}
