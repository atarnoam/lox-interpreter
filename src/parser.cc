#include "src/logging.h"
#include "src/parser.h"

Parser::Parser(std::vector<Token> tokens)
    : tokens(std::move(tokens)), curr(0), m_had_error(false) {}

std::unique_ptr<Expr> Parser::parse() {
    try {
        return expression();
    } catch (const ParseError &error) {
        return nullptr;
    }
}

bool Parser::had_error() const { return m_had_error; }

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

const Token &Parser::consume(TokenType type, const std::string &message) {
    if (!check(type)) {
        throw parse_error(peek(), message);
    }
    return advance();
}

std::unique_ptr<Expr> Parser::expression() { return equality(); }

std::unique_ptr<Expr> Parser::equality() {
    auto expr = comparison();
    while (match(BANG_EQUAL, EQUAL_EQUAL)) {
        Token op = previous();
        auto right = comparison();
        auto left = std::move(expr);
        expr = std::make_unique<Binary>(std::move(left), op, std::move(right));
    }
    return expr;
}

std::unique_ptr<Expr> Parser::comparison() {
    auto expr = term();
    while (match(GREATER, GREATER_EQUAL, LESS, LESS_EQUAL)) {
        Token op = previous();
        auto right = term();
        auto left = std::move(expr);
        expr = std::make_unique<Binary>(std::move(left), op, std::move(right));
    }
    return expr;
}

std::unique_ptr<Expr> Parser::term() {
    auto expr = factor();
    while (match(MINUS, PLUS)) {
        Token op = previous();
        auto right = factor();
        auto left = std::move(expr);
        expr = std::make_unique<Binary>(std::move(left), op, std::move(right));
    }
    return expr;
}

std::unique_ptr<Expr> Parser::factor() {
    auto expr = unary();
    while (match(MINUS, PLUS)) {
        Token op = previous();
        auto right = unary();
        auto left = std::move(expr);
        expr = std::make_unique<Binary>(std::move(left), op, std::move(right));
    }
    return expr;
}

std::unique_ptr<Expr> Parser::unary() {
    if (match(MINUS, BANG)) {
        Token op = previous();
        auto right = unary();
        return std::make_unique<Unary>(op, std::move(right));
    }
    return primary();
}

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
        consume(RIGHT_PAREN, "Expect ')' after expression");
        expr = std::make_unique<Grouping>(std::move(inner_expr));
    } break;
    default:
        throw parse_error(peek(), "Expect expression");
    }
    return expr;
}

Parser::ParseError::ParseError(const std::string &what)
    : std::runtime_error(what) {}

Parser::ParseError Parser::parse_error(const Token &token,
                                       const std::string &message) {
    m_had_error = true;
    auto full_message = report_parse_error(token, message);
    return ParseError(full_message);
}

std::string report_parse_error(const Token &token, const std::string &message) {
    if (token.type == END_OF_FILE) {
        return report(token.line, "at end", message);
    } else {
        return report(token.line, "at '" + token.lexeme + "'", message);
    }
}