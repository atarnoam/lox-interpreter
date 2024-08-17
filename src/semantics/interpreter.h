#pragma once

#include "src/semantics/environment.h"
#include "src/semantics/lox_object.h"
#include "src/semantics/runtime_error.h"
#include "src/syntactics/expr.h"
#include "src/syntactics/stmt.h"

#include <stdexcept>
#include <string>
#include <variant>
#include <vector>

enum class InterpreterMode {
    FILE,
    INTERACTIVE,
};

struct Interpreter : ExprVisitor, StmtVisitor {
    Interpreter();

    LoxObject evaluate(const Expr *expr);
    void execute(const Stmt *stmt);
    void execute_block(const std::vector<std::unique_ptr<Stmt>> &stmts,
                       const std::shared_ptr<Environment> &environment);
    void interpret(const std::vector<std::unique_ptr<Stmt>> &stmts,
                   InterpreterMode mode = InterpreterMode::FILE);

    bool had_runtime_error() const;
    void reset_runtime_error();

  private:
    virtual void visit_assign_expr(const Expr::Assign &assign) override;
    virtual void visit_binary_expr(const Expr::Binary &binary) override;
    virtual void visit_grouping_expr(const Expr::Grouping &grouping) override;
    virtual void visit_literal_expr(const Expr::Literal &literal) override;
    virtual void visit_unary_expr(const Expr::Unary &unary) override;
    virtual void visit_variable_expr(const Expr::Variable &variable) override;

    virtual void visit_block_stmt(const Stmt::Block &block) override;
    virtual void
    visit_expression_stmt(const Stmt::Expression &expression) override;
    virtual void visit_print_stmt(const Stmt::Print &print) override;
    virtual void visit_var_stmt(const Stmt::Var &var) override;

    static void check_numeric_op(const Token &op, const LoxObject &operand);
    static void check_numeric_op(const Token &op, const LoxObject &left,
                                 const LoxObject &right);

    std::shared_ptr<Environment> global_environment;
    std::shared_ptr<Environment> curr_environment;
    LoxObject expr_result;
    bool m_had_runtime_error;
};
