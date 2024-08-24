#include "src/semantics/interpreter.h"

#include "interpreter.h"
#include "src/logging.h"
#include "src/semantics/natives.h"
#include "src/semantics/object/lox_class.h"
#include "src/semantics/object/lox_function.h"
#include "src/semantics/return.h"
#include "src/tp_utils.h"

Interpreter::Interpreter()
    : AbstractInterpreter(), expr_result(LoxNull{}),
      m_had_runtime_error(false) {
    Environment *global_environment = globals();
    for (const auto &[name, obj] : natives) {
        global_environment->define(name, obj);
    }
}

void Interpreter::execute(const Stmt *stmt) {
    if (stmt) {
        stmt->accept(*this);
    }
}

void Interpreter::interpret(const std::vector<std::shared_ptr<Stmt>> &stmts,
                            InterpreterMode mode) {
    if (stmts.empty()) {
        return;
    }

    try {
        std::for_each(stmts.begin(), --stmts.end(),
                      [this](const auto &stmt) { execute(stmt); });

        const Stmt *last_stmt = (--stmts.end())->get();
        execute(last_stmt);

        const Stmt::Expression *expression =
            dynamic_cast<const Stmt::Expression *>(last_stmt);
        if (mode == InterpreterMode::INTERACTIVE && expression != nullptr) {
            print_expr_result();
        }
    } catch (const RuntimeError &err) {
        error(err.token.line, err.what());
        m_had_runtime_error = true;
        curr_environment = globals();
    }
}

bool Interpreter::had_runtime_error() const { return m_had_runtime_error; }
void Interpreter::reset_runtime_error() { m_had_runtime_error = false; }

void Interpreter::visit_assign_expr(const Expr::Assign &assign) {
    LoxObject value = evaluate(assign.value);

    if (locals.contains(&assign)) {
        int distance = locals[&assign];
        curr_environment->assign_at(assign.name, value, distance);
    } else {
        globals()->assign(assign.name, value);
    }
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

void Interpreter::visit_lambda_expr(const Expr::Lambda &func) {
    LoxObject func_object =
        std::make_shared<LoxFunction>(func, curr_environment);
    expr_result = func_object;
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
    evaluate(logical.left);

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

    evaluate(logical.right);
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

void Interpreter::visit_call_expr(const Expr::Call &expr) {
    LoxObject callee = evaluate(expr.callee);

    std::vector<LoxObject> arguments;
    for (const auto &argument_expr : expr.arguments) {
        arguments.push_back(evaluate(argument_expr));
    }
    if (not callee.holds_alternative<std::shared_ptr<LoxCallable>>()) {
        throw RuntimeError(expr.paren, "Can only call functions and classes.");
    }
    auto function = callee.get<std::shared_ptr<LoxCallable>>();

    if (arguments.size() != function->arity()) {
        throw RuntimeError(expr.paren,
                           "Expected " + std::to_string(function->arity()) +
                               " arguments but got " +
                               std::to_string(arguments.size()) + ".");
    }

    expr_result = function->call(*this, arguments);
}

void Interpreter::visit_variable_expr(const Expr::Variable &variable) {
    expr_result = lookup_variable(variable.name, &variable);
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

LoxObject Interpreter::lookup_variable(const Token &name, const Expr *expr) {
    if (!locals.contains(expr)) {
        return globals()->get(name);
    }
    int distance = locals.at(expr);
    return curr_environment->get_at(name, distance);
}

void Interpreter::print_expr_result() {
    std::cout << std::boolalpha << expr_result << std::endl;
}

void Interpreter::visit_block_stmt(const Stmt::Block &block) {
    Environment *new_environment =
        environments.add_environment(curr_environment);
    execute_block(block.statements, new_environment);
}

void Interpreter::visit_class_stmt(const Stmt::Class &stmt) {
    curr_environment->define(stmt.name.lexeme, LoxNull{});
    std::shared_ptr<LoxCallable> lox_class =
        std::make_shared<LoxClass>(stmt.name.lexeme);
    curr_environment->assign(stmt.name, lox_class);
}

void Interpreter::visit_expression_stmt(const Stmt::Expression &expression) {
    evaluate(expression.expression);
}

void Interpreter::visit_function_stmt(const Stmt::Function &func) {
    auto function_object =
        std::make_shared<LoxFunction>(func, curr_environment);
    curr_environment->define(func.name.lexeme, std::move(function_object));
}

void Interpreter::visit_if_stmt(const Stmt::If &stmt) {
    evaluate(stmt.condition);
    if (static_cast<bool>(expr_result)) {
        execute(stmt.then_branch);
    } else {
        execute(stmt.else_branch);
    }
}

void Interpreter::visit_print_stmt(const Stmt::Print &print) {
    evaluate(print.expression);
    print_expr_result();
}

void Interpreter::visit_return_stmt(const Stmt::Return &stmt) {
    LoxObject value;
    if (stmt.value != nullptr) {
        value = evaluate(stmt.value);
    }

    throw Return(value);
}

void Interpreter::visit_var_stmt(const Stmt::Var &var) {
    LoxObject value{LoxNull{}};

    if (var.initializer) {
        value = evaluate(var.initializer);
    }

    curr_environment->define(var.name.lexeme, value);
}

void Interpreter::visit_while_stmt(const Stmt::While &stmt) {
    while (static_cast<bool>(evaluate(stmt.condition))) {
        execute(stmt.body);
    }
}
