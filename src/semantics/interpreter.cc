#include "src/semantics/interpreter.h"

#include "src/logging.h"
#include "src/tp_utils.h"

Interpreter::Interpreter() : result(LoxNull{}), m_had_runtime_error(false) {}

LoxObject Interpreter::evaluate(const Expr *expr) {
    expr->accept(*this);
    return result;
}

void Interpreter::interpret(const Expr *expr) {
    try {
        LoxObject result = evaluate(expr);
        std::cout << std::boolalpha << result << std::endl;
    } catch (const Interpreter::RuntimeError &err) {
        error(err.token.line, err.what());
        m_had_runtime_error = true;
    }
}

bool Interpreter::had_runtime_error() const { return m_had_runtime_error; }

Interpreter::RuntimeError::RuntimeError(Token token, const std::string &what)
    : std::runtime_error(what), token(std::move(token)) {}

void Interpreter::visit_binary_expr(const Expr::Binary &binary) {
    binary.left->accept(*this);
    LoxObject left = result;

    binary.right->accept(*this);
    LoxObject right = result;

    switch (binary.op.type) {
    case MINUS:
        check_numeric_op(binary.op, left, right);
        result = left.get<double>() - right.get<double>();
        break;
    case SLASH:
        check_numeric_op(binary.op, left, right);
        if (right.get<double>() == 0) {
            throw RuntimeError(binary.op, "Division by zero.");
        }
        result = left.get<double>() / right.get<double>();
        break;
    case STAR:
        check_numeric_op(binary.op, left, right);
        result = left.get<double>() * right.get<double>();
        break;
    case PLUS:
        if (left.holds_alternative<double>() &&
            right.holds_alternative<double>()) {
            result = left.get<double>() + right.get<double>();
        } else if (left.holds_alternative<std::string>() &&
                   right.holds_alternative<std::string>()) {
            result = left.get<std::string>() + right.get<std::string>();
        } else {
            throw RuntimeError(binary.op,
                               "Operands must be two numbers or two strings.");
        }
        break;
    case GREATER:
        check_numeric_op(binary.op, left, right);
        result = left > right;
        break;
    case GREATER_EQUAL:
        check_numeric_op(binary.op, left, right);
        result = left >= right;
        break;
    case LESS:
        check_numeric_op(binary.op, left, right);
        result = left < right;
        break;
    case LESS_EQUAL:
        check_numeric_op(binary.op, left, right);
        result = left <= right;
        break;
    case EQUAL_EQUAL:
        result = left == right;
        break;
    case BANG_EQUAL:
        result = left != right;
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
        result = LoxNull{};
        break;
    case TRUE:
        result = true;
        break;
    case FALSE:
        result = false;
        break;
    default:
        if (literal.value.literal.has_value()) {
            result = variant_cast(literal.value.literal.value());
        }
        break;
    }
}

void Interpreter::visit_unary_expr(const Expr::Unary &unary) {
    unary.right->accept(*this);
    switch (unary.op.type) {
    case BANG:
        result = !static_cast<bool>(result);
        break;
    case MINUS:
        check_numeric_op(unary.op, result);
        result = -result.get<double>();
        break;
    default:
        break;
    }
}

void Interpreter::check_numeric_op(const Token &op, const LoxObject &operand) {
    if (operand.holds_alternative<double>()) {
        return;
    }
    throw RuntimeError(op, "Operand must be a number.");
}
void Interpreter::check_numeric_op(const Token &op, const LoxObject &left,
                                   const LoxObject &right) {
    if (left.holds_alternative<double>() && right.holds_alternative<double>()) {
        return;
    }
    throw RuntimeError(op, "Operands must be numbers.");
}
