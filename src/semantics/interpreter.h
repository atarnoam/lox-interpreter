#pragma once

#include "src/semantics/abstract_interpreter.h"
#include "src/semantics/environment.h"
#include "src/semantics/object/lox_callable.h"
#include "src/semantics/object/lox_object.h"
#include "src/semantics/object/print_lox_object.h"
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

struct Interpreter final : AbstractInterpreter, ExprVisitor, StmtVisitor {
    using AbstractInterpreter::execute;

    Interpreter();

    template <typename ExprPtr>
    LoxObject evaluate(const ExprPtr &expr) {
        expr->accept(*this);
        return expr_result;
    }

    void execute(const Stmt *stmt) override;

    void interpret(const std::vector<std::shared_ptr<Stmt>> &stmts,
                   InterpreterMode mode = InterpreterMode::FILE);

    bool had_runtime_error() const;
    void reset_runtime_error();

  private:
    void visit_assign_expr(const Expr::Assign &assign) override;
    void visit_binary_expr(const Expr::Binary &binary) override;
    void visit_call_expr(const Expr::Call &expr) override;
    void visit_get_expr(const Expr::Get &) override;
    void visit_grouping_expr(const Expr::Grouping &grouping) override;
    void visit_lambda_expr(const Expr::Lambda &) override;
    void visit_literal_expr(const Expr::Literal &literal) override;
    void visit_logical_expr(const Expr::Logical &logical) override;
    void visit_set_expr(const Expr::Set &) override;
    void visit_super_expr(const Expr::Super &expr) override;
    void visit_this_expr(const Expr::This &expr) override;
    void visit_unary_expr(const Expr::Unary &unary) override;
    void visit_variable_expr(const Expr::Variable &variable) override;

    void visit_block_stmt(const Stmt::Block &block) override;
    void visit_class_stmt(const Stmt::Class &stmt) override;
    void visit_expression_stmt(const Stmt::Expression &expression) override;
    void visit_function_stmt(const Stmt::Function &) override;
    void visit_if_stmt(const Stmt::If &stmt);
    void visit_print_stmt(const Stmt::Print &print) override;
    void visit_return_stmt(const Stmt::Return &stmt) override;
    void visit_var_stmt(const Stmt::Var &var) override;
    void visit_while_stmt(const Stmt::While &stmt) override;

    static void check_numeric_op(const Token &op, const LoxObject &operand);
    static void check_numeric_op(const Token &op, const LoxObject &left,
                                 const LoxObject &right);

    LoxObject lookup_variable(const Token &name, const Expr *expr);

    void print_expr_result();

    LoxObject expr_result;
    bool m_had_runtime_error;
};
