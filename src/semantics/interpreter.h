#pragma once

#include "src/semantics/lox_object.h"
#include "src/syntactics/expr.h"
#include "src/syntactics/stmt.h"

#include <stdexcept>
#include <string>
#include <variant>
#include <vector>

struct Interpreter : ExprVisitor, StmtVisitor {
    Interpreter();

    LoxObject evaluate(const Expr *expr);
    void execute(const Stmt *stmt);
    void interpret(const std::vector<std::unique_ptr<Stmt>> &stmts);

    bool had_runtime_error() const;
    void reset_runtime_error();

    struct RuntimeError : std::runtime_error {
        explicit RuntimeError(Token token, const std::string &what);

        Token token;
    };

  private:
    virtual void visit_binary_expr(const Expr::Binary &binary) override;
    virtual void visit_grouping_expr(const Expr::Grouping &grouping) override;
    virtual void visit_literal_expr(const Expr::Literal &literal) override;
    virtual void visit_unary_expr(const Expr::Unary &unary) override;

    virtual void
    visit_expression_stmt(const Stmt::Expression &expression) override;
    virtual void visit_print_stmt(const Stmt::Print &print) override;

    static void check_numeric_op(const Token &op, const LoxObject &operand);
    static void check_numeric_op(const Token &op, const LoxObject &left,
                                 const LoxObject &right);

    LoxObject result;
    bool m_had_runtime_error;
};
