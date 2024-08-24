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

struct Resolver : ExprVisitor, StmtVisitor {
    Resolver(AbstractInterpreter &interpreter);

    void resolve(const std::shared_ptr<Expr> &expr);
    void resolve(const std::shared_ptr<Stmt> &stmt);
    void resolve(const std::vector<std::shared_ptr<Stmt>> &stmts);

    bool had_error() const;

  private:
    enum class FunctionType {
        NONE,
        FUNCTION,
    };

    void visit_assign_expr(const Expr::Assign &);
    void visit_binary_expr(const Expr::Binary &);
    void visit_call_expr(const Expr::Call &);
    void visit_grouping_expr(const Expr::Grouping &);
    void visit_literal_expr(const Expr::Literal &);
    void visit_logical_expr(const Expr::Logical &);
    void visit_unary_expr(const Expr::Unary &);
    void visit_variable_expr(const Expr::Variable &);

    void visit_block_stmt(const Stmt::Block &);
    void visit_expression_stmt(const Stmt::Expression &);
    void visit_if_stmt(const Stmt::If &);
    void visit_function_stmt(const Stmt::Function &);
    void visit_print_stmt(const Stmt::Print &);
    void visit_return_stmt(const Stmt::Return &);
    void visit_var_stmt(const Stmt::Var &);
    void visit_while_stmt(const Stmt::While &);

    void begin_scope();
    void end_scope();

    void declare(const Token &var);
    void define(const Token &var);

    void resolve_local(const Expr &expr, const Token &token);
    void resolve_function(const Stmt::Function &function,
                          FunctionType function_type);

    std::string report_resolve_error(const Token &token,
                                     const std::string &message,
                                     bool warning = false);

    AbstractInterpreter &interpreter;
    std::vector<Scope> scopes;
    FunctionType current_function;
    bool m_had_error;
};