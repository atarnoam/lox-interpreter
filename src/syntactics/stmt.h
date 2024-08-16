#pragma once
#include "src/syntactics/expr.h"
#include <memory>

struct Stmt;

struct Expression;
struct Print;

struct StmtVisitor {
    virtual void visit_expression(const Expression &expression) = 0;
    virtual void visit_print(const Print &print) = 0;
};

struct Stmt {
    virtual void accept(StmtVisitor &visitor) const = 0;

    Stmt() = default;
    Stmt(const Stmt &) = default;
    Stmt &operator=(const Stmt &) = default;
    Stmt(Stmt &&) = default;
    Stmt &operator=(Stmt &&) = default;

    virtual ~Stmt() = default;
};

struct Expression : Stmt {
    Expression(std::unique_ptr<Expr> expression);
    virtual void accept(StmtVisitor &visitor) const;
    std::unique_ptr<Expr> expression;
};

struct Print : Stmt {
    Print(std::unique_ptr<Expr> expression);
    virtual void accept(StmtVisitor &visitor) const;
    std::unique_ptr<Expr> expression;
};
