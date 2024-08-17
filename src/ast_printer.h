#pragma once

#include "src/syntactics/expr.h"
#include "src/tp_utils.h"
#include <initializer_list>
#include <memory>
#include <sstream>

struct AstPrinter : ExprVisitor {
    AstPrinter();
    virtual void visit_binary_expr(const Expr::Binary &binary) override;
    virtual void visit_grouping_expr(const Expr::Grouping &grouping) override;
    virtual void visit_literal_expr(const Expr::Literal &literal) override;
    virtual void visit_unary_expr(const Expr::Unary &unary) override;

    std::string get_string() const;

  private:
    template <typename... Args>
    requires are_all_eq<Expr, Args...>
    void parenthesize(const std::string &name, const Args *...exprs) {
        result << "(" << name << " ";
        bool first_iter = true;
        for (const auto &expr : {exprs...}) {
            if (!first_iter) {
                result << " ";
            }
            expr->accept(*this);
            first_iter = false;
        }
        result << ")";
    }
    std::stringstream result;
};