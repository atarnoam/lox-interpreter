#pragma once
#include "src/syntactics/expr.h"
#include "src/syntactics/stmt.fwd.h"
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
    struct Class;
    struct Expression;
    struct If;
    struct Function;
    struct Print;
    struct Return;
    struct Var;
    struct While;
};

struct Stmt::Block : Stmt {
    Block(std::vector<std::shared_ptr<Stmt>> statements);
    virtual void accept(StmtVisitor &visitor) const override;
    std::vector<std::shared_ptr<Stmt>> statements;
};

struct Stmt::Class : Stmt {
    Class(Token name, std::shared_ptr<Expr::Variable> superclass,
          std::vector<std::shared_ptr<Stmt::Function>> methods);
    virtual void accept(StmtVisitor &visitor) const override;
    Token name;
    std::shared_ptr<Expr::Variable> superclass;
    std::vector<std::shared_ptr<Stmt::Function>> methods;
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

struct Stmt::Return : Stmt {
    Return(Token keyword, std::shared_ptr<Expr> value);
    virtual void accept(StmtVisitor &visitor) const override;
    Token keyword;
    std::shared_ptr<Expr> value;
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
    virtual void visit_block_stmt(const Stmt::Block &stmt) = 0;
    virtual void visit_class_stmt(const Stmt::Class &stmt) = 0;
    virtual void visit_expression_stmt(const Stmt::Expression &stmt) = 0;
    virtual void visit_if_stmt(const Stmt::If &stmt) = 0;
    virtual void visit_function_stmt(const Stmt::Function &stmt) = 0;
    virtual void visit_print_stmt(const Stmt::Print &stmt) = 0;
    virtual void visit_return_stmt(const Stmt::Return &stmt) = 0;
    virtual void visit_var_stmt(const Stmt::Var &stmt) = 0;
    virtual void visit_while_stmt(const Stmt::While &stmt) = 0;
};
