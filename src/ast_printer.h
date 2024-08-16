#pragma once

#include "src/expr.h"
#include "src/variadic.h"
#include <initializer_list>
#include <memory>
#include <sstream>

struct AstPrinter : ExprVisitor {
    AstPrinter();
    virtual void visit_binary(const Binary &binary) override;
    virtual void visit_grouping(const Grouping &grouping) override;
    virtual void visit_literal(const Literal &literal) override;
    virtual void visit_unary(const Unary &unary) override;

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