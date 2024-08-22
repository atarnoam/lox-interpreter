#include "parser.h"
#include "src/logging.h"
#include "src/syntactics/parser.h"

Parser::Parser(std::vector<Token> tokens)
    : tokens(std::move(tokens)), curr(0), m_had_error(false) {}

std::vector<std::shared_ptr<Stmt>> Parser::parse() {
    std::vector<std::shared_ptr<Stmt>> statements;
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

std::shared_ptr<Stmt> Parser::declaration() {
    try {
        if (match(VAR)) {
            return var_declaration();
        }
        if (match(FUN)) {
            return function("function");
        }

        return statement();
    } catch (const ParseError &error) {
        synchronize();
        return nullptr;
    }
}

std::shared_ptr<Stmt> Parser::var_declaration() {
    Token name = consume(IDENTIFIER, "Expect variable name.");
    std::shared_ptr<Expr> initializer = nullptr;
    if (match(EQUAL)) {
        initializer = expression();
    }

    consume(SEMICOLON, "Expect ';' after variable declaration.");
    return make_shared<Stmt::Var>(name, std::move(initializer));
}

std::shared_ptr<Stmt> Parser::statement() {
    if (match(IF)) {
        return if_statement();
    }
    if (match(WHILE)) {
        return while_statement();
    }
    if (match(FOR)) {
        return for_statement();
    }

    if (match(PRINT)) {
        return print_statement();
    }
    if (match(LEFT_BRACE)) {
        return make_shared<Stmt::Block>(block_stmt_list());
    }
    return expression_statement();
}

std::vector<std::shared_ptr<Stmt>> Parser::block_stmt_list() {
    std::vector<std::shared_ptr<Stmt>> statements;

    while (!check(RIGHT_BRACE) and !is_at_end()) {
        statements.push_back(std::move(declaration()));
    }

    consume(RIGHT_BRACE, "Expect '}' after block.");
    return statements;
}

std::shared_ptr<Stmt> Parser::if_statement() {
    consume(LEFT_PAREN, "Expect '(' after 'if'.");
    auto condition = expression();
    consume(RIGHT_PAREN, "Expect ')' after if condition.");

    auto then_branch = statement();
    std::shared_ptr<Stmt> else_branch;
    if (match(ELSE)) {
        else_branch = statement();
    }

    return std::make_shared<Stmt::If>(
        std::move(condition), std::move(then_branch), std::move(else_branch));
}

std::shared_ptr<Stmt> Parser::while_statement() {
    consume(LEFT_PAREN, "Expect '(' after 'while'.");
    auto condition = expression();
    consume(RIGHT_PAREN, "Expect ')' after condition.");
    auto body = statement();

    return std::make_shared<Stmt::While>(std::move(condition), std::move(body));
}

std::shared_ptr<Stmt> Parser::for_statement() {
    consume(LEFT_PAREN, "Expect '(' after 'for'.");

    std::shared_ptr<Stmt> initializer;
    if (match(SEMICOLON)) {
        initializer = nullptr;
    } else if (match(VAR)) {
        initializer = var_declaration();
    } else {
        initializer = expression_statement();
    }

    std::shared_ptr<Expr> condition;
    if (check(SEMICOLON)) {
        condition = nullptr;
    } else {
        condition = expression();
    }
    consume(SEMICOLON, "Expect ';' after loop condition.");

    std::shared_ptr<Expr> increment;
    if (!check(RIGHT_PAREN)) {
        increment = expression();
    }
    consume(RIGHT_PAREN, "Expect ')' after loop increment.");

    auto body = statement();

    if (increment) {
        // Extend body
        // Initializer of vectors do not work, so we use two push backs.
        std::vector<std::shared_ptr<Stmt>> extended_body;
        extended_body.push_back(std::move(body));
        extended_body.push_back(
            std::make_shared<Stmt::Expression>(std::move(increment)));
        body = std::make_shared<Stmt::Block>(std::move(extended_body));
    }
    if (not condition) {
        condition = std::make_shared<Expr::Literal>(Token(true));
    }
    body = std::make_shared<Stmt::While>(std::move(condition), std::move(body));
    if (initializer) {
        std::vector<std::shared_ptr<Stmt>> encapsulating_block_list;
        encapsulating_block_list.push_back(std::move(initializer));
        encapsulating_block_list.push_back(std::move(body));
        body =
            std::make_shared<Stmt::Block>(std::move(encapsulating_block_list));
    }
    return body;
}

std::shared_ptr<Stmt> Parser::print_statement() {
    auto expr = expression();
    consume(SEMICOLON, "Expect ; after value.");
    return make_shared<Stmt::Print>(std::move(expr));
}

std::shared_ptr<Stmt> Parser::expression_statement() {
    auto expr = expression();
    consume(SEMICOLON, "Expect ; after value.");
    return make_shared<Stmt::Expression>(std::move(expr));
}

std::shared_ptr<Expr> Parser::expression() { return assignment(); }

std::shared_ptr<Stmt> Parser::function(const std::string &kind) {
    Token name = consume(IDENTIFIER, "Expect " + kind + " name.");
    consume(LEFT_PAREN, "Expect '(' after " + kind + " name.");
    std::vector<Token> parameters;
    if (!check(RIGHT_PAREN)) {
        do {
            if (parameters.size() >= 255) {
                report_parse_error(peek(),
                                   "Can't have more than 255 parameters.");
            }

            parameters.push_back(consume(IDENTIFIER, "Expect parameter name."));
        } while (match(COMMA));
    }
    consume(RIGHT_PAREN, "Expect ')' after parameters.");
    consume(LEFT_BRACE, "Expect '{' before " + kind + " body.");
    auto body = block_stmt_list();
    return std::make_shared<Stmt::Function>(name, parameters, std::move(body));
}

std::shared_ptr<Expr> Parser::assignment() {
    auto expr = or_expr();

    if (match(EQUAL)) {
        Token equals = previous();
        auto value = assignment();

        std::shared_ptr<Expr::Variable> var_expr =
            std::dynamic_pointer_cast<Expr::Variable>(std::move(expr));
        if (var_expr) {
            return make_shared<Expr::Assign>(var_expr->name, std::move(value));
        }

        report_parse_error(equals, "Invalid assignment target.");
    }

    return expr;
}

std::shared_ptr<Expr> Parser::or_expr() {
    auto expr = and_expr();

    while (match(OR)) {
        Token op = previous();
        auto right = and_expr();
        expr = std::make_shared<Expr::Logical>(std::move(expr), op,
                                               std::move(right));
    }

    return expr;
}

std::shared_ptr<Expr> Parser::and_expr() {
    auto expr = equality();

    while (match(AND)) {
        Token op = previous();
        auto right = equality();
        expr = std::make_shared<Expr::Logical>(std::move(expr), op,
                                               std::move(right));
    }

    return expr;
}

std::shared_ptr<Expr> Parser::equality() {
    auto expr = comparison();
    while (match(BANG_EQUAL, EQUAL_EQUAL)) {
        Token op = previous();
        auto right = comparison();
        auto left = std::move(expr);
        expr = std::make_shared<Expr::Binary>(std::move(left), op,
                                              std::move(right));
    }
    return expr;
}

std::shared_ptr<Expr> Parser::comparison() {
    auto expr = term();
    while (match(GREATER, GREATER_EQUAL, LESS, LESS_EQUAL)) {
        Token op = previous();
        auto right = term();
        auto left = std::move(expr);
        expr = std::make_shared<Expr::Binary>(std::move(left), op,
                                              std::move(right));
    }
    return expr;
}

std::shared_ptr<Expr> Parser::term() {
    auto expr = factor();
    while (match(MINUS, PLUS)) {
        Token op = previous();
        auto right = factor();
        auto left = std::move(expr);
        expr = std::make_shared<Expr::Binary>(std::move(left), op,
                                              std::move(right));
    }
    return expr;
}

std::shared_ptr<Expr> Parser::factor() {
    auto expr = unary();
    while (match(STAR, SLASH)) {
        Token op = previous();
        auto right = unary();
        auto left = std::move(expr);
        expr = std::make_shared<Expr::Binary>(std::move(left), op,
                                              std::move(right));
    }
    return expr;
}

std::shared_ptr<Expr> Parser::unary() {
    if (match(MINUS, BANG)) {
        Token op = previous();
        auto right = unary();
        return std::make_shared<Expr::Unary>(op, std::move(right));
    }
    return call();
}

std::shared_ptr<Expr> Parser::call() {
    auto expr = primary();
    while (true) {
        if (match(LEFT_PAREN)) {
            expr = finish_call(std::move(expr));
        } else {
            break;
        }
    }

    return expr;
}

std::shared_ptr<Expr> Parser::primary() {
    auto token = advance();
    std::shared_ptr<Expr> expr;
    switch (token.type) {
    case NUMBER:
    case STRING:
    case TRUE:
    case FALSE:
    case NIL:
        expr = std::make_shared<Expr::Literal>(token);
        break;
    case LEFT_PAREN: {
        auto inner_expr = expression();
        consume(RIGHT_PAREN, "Expect ')' after expression");
        expr = std::make_shared<Expr::Grouping>(std::move(inner_expr));
    } break;
    case IDENTIFIER:
        expr = std::make_shared<Expr::Variable>(token);
        break;
    default:
        throw parse_error(peek(), "Expect expression");
    }
    return expr;
}

std::shared_ptr<Expr> Parser::finish_call(std::shared_ptr<Expr> callee) {
    std::vector<std::shared_ptr<Expr>> arguments;
    if (!check(RIGHT_PAREN)) {
        do {
            if (arguments.size() > 255) {
                report_parse_error(peek(),
                                   "Can't have more than 255 arguments.");
            }
            arguments.push_back(expression());
        } while (match(COMMA));
    }

    Token paren = consume(RIGHT_PAREN, "Expect ')' after arguments.");

    return make_shared<Expr::Call>(std::move(callee), paren,
                                   std::move(arguments));
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