#pragma once
#include "src/syntactics/token.h"
#include <memory>

struct Expr;

struct Binary;
struct Grouping;
struct Literal;
struct Unary;

struct ExprVisitor {
    virtual void visit_binary(const Binary &binary) = 0;
    virtual void visit_grouping(const Grouping &grouping) = 0;
    virtual void visit_literal(const Literal &literal) = 0;
    virtual void visit_unary(const Unary &unary) = 0;
};

struct Expr { 
    virtual void accept(ExprVisitor &visitor) const = 0;
    
    Expr() = default;
    Expr(const Expr &) = default;
    Expr &operator=(const Expr &) = default;
    Expr(Expr &&) = default;
    Expr &operator=(Expr &&) = default;
    
    virtual ~Expr() = default;
};

struct Binary : Expr {
    Binary(std::unique_ptr<Expr> left, Token op, std::unique_ptr<Expr> right);
    virtual void accept(ExprVisitor& visitor) const;
    std::unique_ptr<Expr> left;
    Token op;
    std::unique_ptr<Expr> right;
};

struct Grouping : Expr {
    Grouping(std::unique_ptr<Expr> expression);
    virtual void accept(ExprVisitor& visitor) const;
    std::unique_ptr<Expr> expression;
};

struct Literal : Expr {
    Literal(Token value);
    virtual void accept(ExprVisitor& visitor) const;
    Token value;
};

struct Unary : Expr {
    Unary(Token op, std::unique_ptr<Expr> right);
    virtual void accept(ExprVisitor& visitor) const;
    Token op;
    std::unique_ptr<Expr> right;
};

