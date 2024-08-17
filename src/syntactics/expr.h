#pragma once
#include "src/syntactics/token.h"
#include <memory>

struct ExprVisitor;

struct Expr {
    virtual void accept(ExprVisitor &visitor) const = 0;

    Expr() = default;
    Expr(const Expr &) = default;
    Expr &operator=(const Expr &) = default;
    Expr(Expr &&) = default;
    Expr &operator=(Expr &&) = default;

    virtual ~Expr() = default;

    struct Binary;
    struct Grouping;
    struct Literal;
    struct Unary;
};

struct Expr::Binary : Expr {
    Binary(std::unique_ptr<Expr> left, Token op, std::unique_ptr<Expr> right);
    virtual void accept(ExprVisitor &visitor) const override;
    std::unique_ptr<Expr> left;
    Token op;
    std::unique_ptr<Expr> right;
};

struct Expr::Grouping : Expr {
    Grouping(std::unique_ptr<Expr> expression);
    virtual void accept(ExprVisitor &visitor) const override;
    std::unique_ptr<Expr> expression;
};

struct Expr::Literal : Expr {
    Literal(Token value);
    virtual void accept(ExprVisitor &visitor) const override;
    Token value;
};

struct Expr::Unary : Expr {
    Unary(Token op, std::unique_ptr<Expr> right);
    virtual void accept(ExprVisitor &visitor) const override;
    Token op;
    std::unique_ptr<Expr> right;
};

struct ExprVisitor {
    virtual void visit_binary_expr(const Expr::Binary &binary) = 0;
    virtual void visit_grouping_expr(const Expr::Grouping &grouping) = 0;
    virtual void visit_literal_expr(const Expr::Literal &literal) = 0;
    virtual void visit_unary_expr(const Expr::Unary &unary) = 0;
};
