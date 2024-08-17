#pragma once
#include "src/syntactics/expr.h"
#include <memory>

struct StmtVisitor;

struct Stmt {
    virtual void accept(StmtVisitor &visitor) const = 0;

    Stmt() = default;
    Stmt(const Stmt &) = default;
    Stmt &operator=(const Stmt &) = default;
    Stmt(Stmt &&) = default;
    Stmt &operator=(Stmt &&) = default;

    virtual ~Stmt() = default;

    struct Expression;
    struct Print;
    struct Var;
};

struct Stmt::Expression : Stmt {
    Expression(std::unique_ptr<Expr> expression);
    virtual void accept(StmtVisitor &visitor) const override;
    std::unique_ptr<Expr> expression;
};

struct Stmt::Print : Stmt {
    Print(std::unique_ptr<Expr> expression);
    virtual void accept(StmtVisitor &visitor) const override;
    std::unique_ptr<Expr> expression;
};

struct Stmt::Var : Stmt {
    Var(Token name, std::unique_ptr<Expr> initializer);
    virtual void accept(StmtVisitor &visitor) const override;
    Token name;
    std::unique_ptr<Expr> initializer;
};

struct StmtVisitor {
    virtual void visit_expression_stmt(const Stmt::Expression &expression) = 0;
    virtual void visit_print_stmt(const Stmt::Print &print) = 0;
    virtual void visit_var_stmt(const Stmt::Var &var) = 0;
};
