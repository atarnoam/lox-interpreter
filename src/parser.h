#pragma once

#include "src/expr.h"
#include "src/token.h"
#include "src/variadic.h"
#include <memory>
#include <vector>

struct Parser {
    Parser(std::vector<Token> tokens);

    bool is_at_end() const;

    template <typename... Args> bool match(Args... token_types) {
        static_assert(are_all_eq<TokenType, Args...>());
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

    std::unique_ptr<Expr> expression();

    bool had_error;

  private:
    std::unique_ptr<Expr> equality();
    std::unique_ptr<Expr> comparison();
    std::unique_ptr<Expr> primary();

    std::vector<Token> tokens;
    size_t curr;
};