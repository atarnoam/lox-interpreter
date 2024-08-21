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

    struct Assign;
    struct Binary;
    struct Grouping;
    struct Literal;
    struct Logical;
    struct Unary;
    struct Variable;
};

struct Expr::Assign : Expr {
    Assign(Token name, std::shared_ptr<Expr> value);
    virtual void accept(ExprVisitor &visitor) const override;
    Token name;
    std::shared_ptr<Expr> value;
};

struct Expr::Binary : Expr {
    Binary(std::shared_ptr<Expr> left, Token op, std::shared_ptr<Expr> right);
    virtual void accept(ExprVisitor &visitor) const override;
    std::shared_ptr<Expr> left;
    Token op;
    std::shared_ptr<Expr> right;
};

struct Expr::Grouping : Expr {
    Grouping(std::shared_ptr<Expr> expression);
    virtual void accept(ExprVisitor &visitor) const override;
    std::shared_ptr<Expr> expression;
};

struct Expr::Literal : Expr {
    Literal(Token value);
    virtual void accept(ExprVisitor &visitor) const override;
    Token value;
};

struct Expr::Logical : Expr {
    Logical(std::shared_ptr<Expr> left, Token op, std::shared_ptr<Expr> right);
    virtual void accept(ExprVisitor &visitor) const override;
    std::shared_ptr<Expr> left;
    Token op;
    std::shared_ptr<Expr> right;
};

struct Expr::Unary : Expr {
    Unary(Token op, std::shared_ptr<Expr> right);
    virtual void accept(ExprVisitor &visitor) const override;
    Token op;
    std::shared_ptr<Expr> right;
};

struct Expr::Variable : Expr {
    Variable(Token name);
    virtual void accept(ExprVisitor &visitor) const override;
    Token name;
};

struct ExprVisitor {
    virtual void visit_assign_expr(const Expr::Assign &) = 0;
    virtual void visit_binary_expr(const Expr::Binary &) = 0;
    virtual void visit_grouping_expr(const Expr::Grouping &) = 0;
    virtual void visit_literal_expr(const Expr::Literal &) = 0;
    virtual void visit_logical_expr(const Expr::Logical &) = 0;
    virtual void visit_unary_expr(const Expr::Unary &) = 0;
    virtual void visit_variable_expr(const Expr::Variable &) = 0;
};
