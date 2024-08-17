#include "parser.h"
#include "src/logging.h"
#include "src/syntactics/parser.h"

Parser::Parser(std::vector<Token> tokens)
    : tokens(std::move(tokens)), curr(0), m_had_error(false) {}

std::vector<std::unique_ptr<Stmt>> Parser::parse() {
    std::vector<std::unique_ptr<Stmt>> statements;
    while (!is_at_end()) {
        try {
            statements.push_back(std::move(declaration()));
        } catch (const ParseError &error) {
        }
    }
    return statements;
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

std::unique_ptr<Stmt> Parser::declaration() {
    try {
        if (match(VAR)) {
            return var_declaration();
        }

        return statement();
    } catch (const ParseError &error) {
        synchronize();
        return nullptr;
    }
}

std::unique_ptr<Stmt> Parser::var_declaration() {
    Token name = consume(IDENTIFIER, "Expect variable name.");
    std::unique_ptr<Expr> initializer = nullptr;
    if (match(EQUAL)) {
        initializer = expression();
    }

    consume(SEMICOLON, "Expect ';' after variable declaration.");
    return make_unique<Stmt::Var>(name, std::move(initializer));
}

std::unique_ptr<Stmt> Parser::statement() {
    if (match(PRINT)) {
        return print_statement();
    }
    if (match(LEFT_BRACE)) {
        return make_unique<Stmt::Block>(block_stmt_list());
    }
    return expression_statement();
}

std::vector<std::unique_ptr<Stmt>> Parser::block_stmt_list() {
    std::vector<std::unique_ptr<Stmt>> statements;

    while (!check(RIGHT_BRACE) && !is_at_end()) {
        statements.push_back(std::move(declaration()));
    }

    consume(RIGHT_BRACE, "Expect '}' after block.");
    return statements;
}

std::unique_ptr<Stmt> Parser::print_statement() {
    auto expr = expression();
    consume(SEMICOLON, "Expect ; after value.");
    return make_unique<Stmt::Print>(std::move(expr));
}

std::unique_ptr<Stmt> Parser::expression_statement() {
    auto expr = expression();
    consume(SEMICOLON, "Expect ; after value.");
    return make_unique<Stmt::Expression>(std::move(expr));
}

std::unique_ptr<Expr> Parser::expression() { return assignment(); }
std::unique_ptr<Expr> Parser::assignment() {
    auto expr = equality();

    if (match(EQUAL)) {
        Token equals = previous();
        auto value = assignment();

        std::unique_ptr<Expr::Variable> var_expr =
            cast_to<Expr::Variable, Expr>(std::move(expr));
        if (var_expr) {
            return make_unique<Expr::Assign>(var_expr->name, std::move(value));
        }

        report_parse_error(equals, "Invalid assignment target.");
    }

    return expr;
}

std::unique_ptr<Expr> Parser::equality() {
    auto expr = comparison();
    while (match(BANG_EQUAL, EQUAL_EQUAL)) {
        Token op = previous();
        auto right = comparison();
        auto left = std::move(expr);
        expr = std::make_unique<Expr::Binary>(std::move(left), op,
                                              std::move(right));
    }
    return expr;
}

std::unique_ptr<Expr> Parser::comparison() {
    auto expr = term();
    while (match(GREATER, GREATER_EQUAL, LESS, LESS_EQUAL)) {
        Token op = previous();
        auto right = term();
        auto left = std::move(expr);
        expr = std::make_unique<Expr::Binary>(std::move(left), op,
                                              std::move(right));
    }
    return expr;
}

std::unique_ptr<Expr> Parser::term() {
    auto expr = factor();
    while (match(MINUS, PLUS)) {
        Token op = previous();
        auto right = factor();
        auto left = std::move(expr);
        expr = std::make_unique<Expr::Binary>(std::move(left), op,
                                              std::move(right));
    }
    return expr;
}

std::unique_ptr<Expr> Parser::factor() {
    auto expr = unary();
    while (match(STAR, SLASH)) {
        Token op = previous();
        auto right = unary();
        auto left = std::move(expr);
        expr = std::make_unique<Expr::Binary>(std::move(left), op,
                                              std::move(right));
    }
    return expr;
}

std::unique_ptr<Expr> Parser::unary() {
    if (match(MINUS, BANG)) {
        Token op = previous();
        auto right = unary();
        return std::make_unique<Expr::Unary>(op, std::move(right));
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
        expr = std::make_unique<Expr::Literal>(token);
        break;
    case LEFT_PAREN: {
        auto inner_expr = expression();
        consume(RIGHT_PAREN, "Expect ')' after expression");
        expr = std::make_unique<Expr::Grouping>(std::move(inner_expr));
    } break;
    case IDENTIFIER:
        expr = std::make_unique<Expr::Variable>(token);
        break;
    default:
        throw parse_error(peek(), "Expect expression");
    }
    return expr;
}

Parser::ParseError::ParseError(const std::string &what)
    : std::runtime_error(what) {}

Parser::ParseError Parser::parse_error(const Token &token,
                                       const std::string &message) {
    auto full_message = report_parse_error(token, message);
    return ParseError(full_message);
}

void Parser::synchronize() {
    advance();

    while (!is_at_end()) {
        if (previous().type == SEMICOLON) {
            return;
        }

        switch (peek().type) {
        case CLASS:
        case FUN:
        case VAR:
        case FOR:
        case IF:
        case WHILE:
        case PRINT:
        case RETURN:
            return;
        default:
            break;
        }

        advance();
    }
}

std::string Parser::report_parse_error(const Token &token,
                                       const std::string &message) {
    m_had_error = true;
    if (token.type == END_OF_FILE) {
        return report(token.line, "at end", message);
    } else {
        return report(token.line, "at '" + token.lexeme + "'", message);
    }
}