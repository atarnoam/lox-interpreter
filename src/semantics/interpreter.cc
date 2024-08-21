#include "src/semantics/interpreter.h"

#include "interpreter.h"
#include "src/logging.h"
#include "src/tp_utils.h"

Interpreter::Interpreter()
    : global_environment(std::make_shared<Environment>()),
      curr_environment(global_environment), expr_result(LoxNull{}),
      m_had_runtime_error(false) {}

LoxObject Interpreter::evaluate(const Expr *expr) {
    expr->accept(*this);
    return expr_result;
}

void Interpreter::execute(const Stmt *stmt) {
    if (stmt) {
        stmt->accept(*this);
    }
}

void Interpreter::execute_block(
    const std::vector<std::shared_ptr<Stmt>> &stmts,
    const std::shared_ptr<Environment> &environment) {
    auto previous_environment = curr_environment;
    curr_environment = environment;
    for (const auto &stmt : stmts) {
        execute(stmt.get());
    }
    curr_environment = previous_environment;
}

void Interpreter::interpret(const std::vector<std::shared_ptr<Stmt>> &stmts,
                            InterpreterMode mode) {
    if (stmts.empty()) {
        return;
    }

    try {
        std::for_each(stmts.begin(), --stmts.end(),
                      [this](const auto &stmt) { execute(stmt.get()); });

        const Stmt *last_stmt = (--stmts.end())->get();
        execute(last_stmt);

        const Stmt::Expression *expression =
            dynamic_cast<const Stmt::Expression *>(last_stmt);
        if (mode == InterpreterMode::INTERACTIVE && expression != nullptr) {
            std::cout << expr_result << std::endl;
        }
    } catch (const RuntimeError &err) {
        error(err.token.line, err.what());
        m_had_runtime_error = true;
        curr_environment = global_environment;
    }
}

bool Interpreter::had_runtime_error() const { return m_had_runtime_error; }
void Interpreter::reset_runtime_error() { m_had_runtime_error = false; }

void Interpreter::visit_assign_expr(const Expr::Assign &assign) {
    LoxObject value = evaluate(assign.value.get());
    curr_environment->assign(assign.name, value);
    expr_result = value;
}

void Interpreter::visit_binary_expr(const Expr::Binary &binary) {
    binary.left->accept(*this);
    LoxObject left = expr_result;

    binary.right->accept(*this);
    LoxObject right = expr_result;

    switch (binary.op.type) {
    case MINUS:
        check_numeric_op(binary.op, left, right);
        expr_result = left.get<double>() - right.get<double>();
        break;
    case SLASH:
        check_numeric_op(binary.op, left, right);
        if (right.get<double>() == 0) {
            throw RuntimeError(binary.op, "Division by zero.");
        }
        expr_result = left.get<double>() / right.get<double>();
        break;
    case STAR:
        check_numeric_op(binary.op, left, right);
        expr_result = left.get<double>() * right.get<double>();
        break;
    case PLUS:
        if (left.holds_alternative<double>() &&
            right.holds_alternative<double>()) {
            expr_result = left.get<double>() + right.get<double>();
        } else if (left.holds_alternative<std::string>() &&
                   right.holds_alternative<std::string>()) {
            expr_result = left.get<std::string>() + right.get<std::string>();
        } else {
            throw RuntimeError(binary.op,
                               "Operands must be two numbers or two strings.");
        }
        break;
    case GREATER:
        check_numeric_op(binary.op, left, right);
        expr_result = left > right;
        break;
    case GREATER_EQUAL:
        check_numeric_op(binary.op, left, right);
        expr_result = left >= right;
        break;
    case LESS:
        check_numeric_op(binary.op, left, right);
        expr_result = left < right;
        break;
    case LESS_EQUAL:
        check_numeric_op(binary.op, left, right);
        expr_result = left <= right;
        break;
    case EQUAL_EQUAL:
        expr_result = left == right;
        break;
    case BANG_EQUAL:
        expr_result = left != right;
        break;
    default:
        break;
    }
}

void Interpreter::visit_grouping_expr(const Expr::Grouping &grouping) {
    grouping.expression->accept(*this);
}

void Interpreter::visit_literal_expr(const Expr::Literal &literal) {
    switch (literal.value.type) {
    case NIL:
        expr_result = LoxNull{};
        break;
    case TRUE:
        expr_result = true;
        break;
    case FALSE:
        expr_result = false;
        break;
    default:
        if (literal.value.literal.has_value()) {
            expr_result = variant_cast(literal.value.literal.value());
        }
        break;
    }
}

void Interpreter::visit_logical_expr(const Expr::Logical &logical) {
    evaluate(logical.left.get());

    // Short circuiting
    if (logical.op.type == OR) {
        if (static_cast<bool>(expr_result)) {
            return;
        }
    } else {
        if (!static_cast<bool>(expr_result)) {
            return;
        }
    }

    evaluate(logical.right.get());
}

void Interpreter::visit_unary_expr(const Expr::Unary &unary) {
    unary.right->accept(*this);
    switch (unary.op.type) {
    case BANG:
        expr_result = !static_cast<bool>(expr_result);
        break;
    case MINUS:
        check_numeric_op(unary.op, expr_result);
        expr_result = -expr_result.get<double>();
        break;
    default:
        break;
    }
}

void Interpreter::visit_variable_expr(const Expr::Variable &variable) {
    expr_result = curr_environment->get(variable.name);
}

void Interpreter::check_numeric_op(const Token &op, const LoxObject &operand) {
    if (operand.holds_alternative<double>()) {
        return;
    }
    throw RuntimeError(op, "Operand must be a number.");
}
void Interpreter::check_numeric_op(const Token &op, const LoxObject &left,
                                   const LoxObject &right) {
    if (left.holds_alternative<double>() and
        right.holds_alternative<double>()) {
        return;
    }
    throw RuntimeError(op, "Operands must be numbers.");
}

void Interpreter::visit_block_stmt(const Stmt::Block &block) {
    execute_block(block.statements,
                  std::make_shared<Environment>(curr_environment));
}

void Interpreter::visit_expression_stmt(const Stmt::Expression &expression) {
    evaluate(expression.expression.get());
}

void Interpreter::visit_if_stmt(const Stmt::If &stmt) {
    evaluate(stmt.condition.get());
    if (static_cast<bool>(expr_result)) {
        execute(stmt.then_branch.get());
    } else {
        execute(stmt.else_branch.get());
    }
}

void Interpreter::visit_print_stmt(const Stmt::Print &print) {
    evaluate(print.expression.get());
    std::cout << std::boolalpha << expr_result << std::endl;
}

void Interpreter::visit_var_stmt(const Stmt::Var &var) {
    LoxObject value{LoxNull{}};

    if (var.initializer) {
        value = evaluate(var.initializer.get());
    }

    curr_environment->define(var.name.lexeme, value);
}

void Interpreter::visit_while_stmt(const Stmt::While &stmt) {
    while (static_cast<bool>(evaluate(stmt.condition.get()))) {
        execute(stmt.body.get());
    }
}
