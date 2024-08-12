#include "src/parser.h"

Parser::Parser(std::vector<Token> tokens)
    : had_error(false), tokens(std::move(tokens)), curr(0) {}

bool Parser::is_at_end() const { return peek().type == END_OF_FILE; }

const Token &Parser::peek() const { return tokens[curr]; }
const Token &Parser::previous() const {
    return tokens[curr == 0 ? curr : curr - 1];
}
bool Parser::check(TokenType token_type) const {
    return peek().type == token_type;
}
const Token &Parser::advance() {
    if (!is_at_end()) {
        return tokens[curr++];
    }
    return tokens[curr];
}

std::unique_ptr<Expr> Parser::expression() { return equality(); }

std::unique_ptr<Expr> Parser::equality() {
    // auto expr = comparison();
    auto expr = primary();
    while (match(BANG_EQUAL, EQUAL_EQUAL)) {
        Token op = previous();
        auto right = primary();
        auto left = std::move(expr);
        expr = std::make_unique<Binary>(std::move(left), op, std::move(right));
    }
    return expr;
}
std::unique_ptr<Expr> Parser::comparison() { return nullptr; }
std::unique_ptr<Expr> Parser::primary() {
    auto token = advance();
    std::unique_ptr<Expr> expr;
    switch (token.type) {
    case NUMBER:
    case STRING:
    case TRUE:
    case FALSE:
    case NIL:
        expr = std::make_unique<Literal>(token);
        break;
    case LEFT_PAREN: {
        auto inner_expr = expression();
        if (advance().type != RIGHT_PAREN) {
            had_error = true;
            return nullptr;
        }
        expr = std::make_unique<Grouping>(std::move(inner_expr));
    } break;
    default:
        had_error = true;
    }
    return expr;
}