#pragma once
#include "src/syntactics/stmt.fwd.h"
#include "src/syntactics/token.h"
#include <memory>
#include <vector>

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
    struct Call;
    struct Get;
    struct Grouping;
    struct Lambda;
    struct Literal;
    struct Logical;
    struct Set;
    struct This;
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

struct Expr::Call : Expr {
    Call(std::shared_ptr<Expr> callee, Token paren,
         std::vector<std::shared_ptr<Expr>> arguments);
    virtual void accept(ExprVisitor &visitor) const override;
    std::shared_ptr<Expr> callee;
    Token paren;
    std::vector<std::shared_ptr<Expr>> arguments;
};

struct Expr::Get : Expr {
    Get(std::shared_ptr<Expr> object, Token name);
    virtual void accept(ExprVisitor &visitor) const override;
    std::shared_ptr<Expr> object;
    Token name;
};

struct Expr::Grouping : Expr {
    Grouping(std::shared_ptr<Expr> expression);
    virtual void accept(ExprVisitor &visitor) const override;
    std::shared_ptr<Expr> expression;
};

struct Expr::Lambda : Expr {
    Lambda(Token keyword, std::vector<Token> params,
           std::vector<std::shared_ptr<Stmt>> body);
    virtual void accept(ExprVisitor &visitor) const override;
    Token keyword;
    std::vector<Token> params;
    std::vector<std::shared_ptr<Stmt>> body;
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

struct Expr::Set : Expr {
    Set(std::shared_ptr<Expr> object, Token name, std::shared_ptr<Expr> value);
    virtual void accept(ExprVisitor &visitor) const override;
    std::shared_ptr<Expr> object;
    Token name;
    std::shared_ptr<Expr> value;
};

struct Expr::This : Expr {
    This(Token keyword);
    virtual void accept(ExprVisitor &visitor) const override;
    Token keyword;
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
    virtual void visit_assign_expr(const Expr::Assign &expr) = 0;
    virtual void visit_binary_expr(const Expr::Binary &expr) = 0;
    virtual void visit_call_expr(const Expr::Call &expr) = 0;
    virtual void visit_get_expr(const Expr::Get &expr) = 0;
    virtual void visit_grouping_expr(const Expr::Grouping &expr) = 0;
    virtual void visit_lambda_expr(const Expr::Lambda &expr) = 0;
    virtual void visit_literal_expr(const Expr::Literal &expr) = 0;
    virtual void visit_logical_expr(const Expr::Logical &expr) = 0;
    virtual void visit_set_expr(const Expr::Set &expr) = 0;
    virtual void visit_this_expr(const Expr::This &expr) = 0;
    virtual void visit_unary_expr(const Expr::Unary &expr) = 0;
    virtual void visit_variable_expr(const Expr::Variable &expr) = 0;
};
