#pragma once
#include "src/syntactics/expr.h"
#include <memory>
#include <vector>

struct StmtVisitor;

struct Stmt {
    virtual void accept(StmtVisitor &visitor) const = 0;

    Stmt() = default;
    Stmt(const Stmt &) = default;
    Stmt &operator=(const Stmt &) = default;
    Stmt(Stmt &&) = default;
    Stmt &operator=(Stmt &&) = default;

    virtual ~Stmt() = default;

    struct Block;
    struct Expression;
    struct If;
    struct Function;
    struct Print;
    struct Var;
    struct While;
};

struct Stmt::Block : Stmt {
    Block(std::vector<std::shared_ptr<Stmt>> statements);
    virtual void accept(StmtVisitor &visitor) const override;
    std::vector<std::shared_ptr<Stmt>> statements;
};

struct Stmt::Expression : Stmt {
    Expression(std::shared_ptr<Expr> expression);
    virtual void accept(StmtVisitor &visitor) const override;
    std::shared_ptr<Expr> expression;
};

struct Stmt::If : Stmt {
    If(std::shared_ptr<Expr> condition, std::shared_ptr<Stmt> then_branch,
       std::shared_ptr<Stmt> else_branch);
    virtual void accept(StmtVisitor &visitor) const override;
    std::shared_ptr<Expr> condition;
    std::shared_ptr<Stmt> then_branch;
    std::shared_ptr<Stmt> else_branch;
};

struct Stmt::Function : Stmt {
    Function(Token name, std::vector<Token> params,
             std::vector<std::shared_ptr<Stmt>> body);
    virtual void accept(StmtVisitor &visitor) const override;
    Token name;
    std::vector<Token> params;
    std::vector<std::shared_ptr<Stmt>> body;
};

struct Stmt::Print : Stmt {
    Print(std::shared_ptr<Expr> expression);
    virtual void accept(StmtVisitor &visitor) const override;
    std::shared_ptr<Expr> expression;
};

struct Stmt::Var : Stmt {
    Var(Token name, std::shared_ptr<Expr> initializer);
    virtual void accept(StmtVisitor &visitor) const override;
    Token name;
    std::shared_ptr<Expr> initializer;
};

struct Stmt::While : Stmt {
    While(std::shared_ptr<Expr> condition, std::shared_ptr<Stmt> body);
    virtual void accept(StmtVisitor &visitor) const override;
    std::shared_ptr<Expr> condition;
    std::shared_ptr<Stmt> body;
};

struct StmtVisitor {
    virtual void visit_block_stmt(const Stmt::Block &) = 0;
    virtual void visit_expression_stmt(const Stmt::Expression &) = 0;
    virtual void visit_if_stmt(const Stmt::If &) = 0;
    virtual void visit_function_stmt(const Stmt::Function &) = 0;
    virtual void visit_print_stmt(const Stmt::Print &) = 0;
    virtual void visit_var_stmt(const Stmt::Var &) = 0;
    virtual void visit_while_stmt(const Stmt::While &) = 0;
};
