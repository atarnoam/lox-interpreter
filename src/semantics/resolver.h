#pragma once

#include "src/semantics/abstract_interpreter.h"
#include "src/syntactics/expr.h"
#include "src/syntactics/stmt.h"

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

enum class VariableStatus { UNKNOWN, DECLARED, DEFINED };

struct Scope {
    Scope() = default;

    void declare(const std::string &name);
    void define(const std::string &name);

    bool in_scope(const std::string &name) const;
    VariableStatus check_scope(const std::string &name) const;

  private:
    std::unordered_map<std::string, bool> map;
};

struct Resolver final : ExprVisitor, StmtVisitor {
    Resolver(AbstractInterpreter &interpreter);

    void resolve(const std::shared_ptr<Expr> &expr);
    void resolve(const std::shared_ptr<Stmt> &stmt);
    void resolve(const std::vector<std::shared_ptr<Stmt>> &stmts);

    bool had_error() const;

  private:
    enum class FunctionType {
        NONE,
        FUNCTION,
        INITIALIZER,
        METHOD,
    };

    enum class ClassType { NONE, CLASS, SUBCLASS };

    void visit_assign_expr(const Expr::Assign &expr) override;
    void visit_binary_expr(const Expr::Binary &expr) override;
    void visit_call_expr(const Expr::Call &expr) override;
    void visit_get_expr(const Expr::Get &expr) override;
    void visit_grouping_expr(const Expr::Grouping &expr) override;
    void visit_lambda_expr(const Expr::Lambda &expr) override;
    void visit_literal_expr(const Expr::Literal &expr) override;
    void visit_logical_expr(const Expr::Logical &expr) override;
    void visit_set_expr(const Expr::Set &expr) override;
    void visit_super_expr(const Expr::Super &expr) override;
    void visit_this_expr(const Expr::This &expr) override;
    void visit_unary_expr(const Expr::Unary &expr) override;
    void visit_variable_expr(const Expr::Variable &expr) override;

    void visit_block_stmt(const Stmt::Block &stmt) override;
    void visit_class_stmt(const Stmt::Class &stmt) override;
    void visit_expression_stmt(const Stmt::Expression &stmt) override;
    void visit_if_stmt(const Stmt::If &stmt) override;
    void visit_function_stmt(const Stmt::Function &stmt) override;
    void visit_print_stmt(const Stmt::Print &stmt) override;
    void visit_return_stmt(const Stmt::Return &stmt) override;
    void visit_var_stmt(const Stmt::Var &stmt) override;
    void visit_while_stmt(const Stmt::While &stmt) override;

    void begin_scope();
    void end_scope();

    void declare(const Token &var);
    void define(const Token &var);

    void resolve_local(const Expr &expr, const Token &token);

    void resolve_function(const Stmt::Function &function, FunctionType type);
    void resolve_function(const std::vector<Token> &params,
                          const std::vector<std::shared_ptr<Stmt>> &body,
                          FunctionType type);

    std::string report_resolve_error(const Token &token,
                                     const std::string &message,
                                     bool warning = false);

    AbstractInterpreter &interpreter;
    std::vector<Scope> scopes;
    FunctionType current_function;
    ClassType current_class;
    bool m_had_error;
};