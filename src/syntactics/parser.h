#pragma once

#include "src/syntactics/expr.h"
#include "src/syntactics/token.h"
#include "src/variadic.h"

#include <memory>
#include <stdexcept>
#include <vector>

struct Parser {
    explicit Parser(std::vector<Token> tokens);

    std::unique_ptr<Expr> parse();
    struct ParseError : std::runtime_error {
        explicit ParseError(const std::string &what);
    };
    bool had_error() const;

  private:
    bool is_at_end() const;

    template <typename... Args>
    bool match(Args... token_types) requires are_all_eq<TokenType, Args...> {
        for (auto token_type : {token_types...}) {
            if (check(token_type)) {
                advance();
                return true;
            }
        }
        return false;
    }
    bool check(TokenType token_type) const;
    const Token &peek() const;
    const Token &previous() const;
    const Token &advance();
    const Token &consume(TokenType type, const std::string &message);

    std::unique_ptr<Expr> expression();

    std::unique_ptr<Expr> equality();
    std::unique_ptr<Expr> comparison();
    std::unique_ptr<Expr> term();
    std::unique_ptr<Expr> factor();
    std::unique_ptr<Expr> unary();
    std::unique_ptr<Expr> primary();
    ParseError parse_error(const Token &token, const std::string &message);

    std::vector<Token> tokens;
    size_t curr;
    bool m_had_error;
};

std::string report_parse_error(const Token &token, const std::string &message);
