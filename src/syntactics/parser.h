#pragma once

#include "src/syntactics/expr.h"
#include "src/syntactics/stmt.h"
#include "src/syntactics/token.h"
#include "src/tp_utils.h"

#include <memory>
#include <stdexcept>
#include <vector>

struct Parser {
    explicit Parser(std::vector<Token> tokens);

    std::vector<std::shared_ptr<Stmt>> parse();
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

    std::shared_ptr<Stmt> declaration();
    std::shared_ptr<Stmt> var_declaration();
    std::shared_ptr<Stmt> function(const std::string &kind);
    std::shared_ptr<Stmt> statement();
    std::vector<std::shared_ptr<Stmt>> block_stmt_list();
    std::shared_ptr<Stmt> if_statement();
    std::shared_ptr<Stmt> while_statement();
    std::shared_ptr<Stmt> for_statement();
    std::shared_ptr<Stmt> print_statement();
    std::shared_ptr<Stmt> expression_statement();

    std::shared_ptr<Expr> expression();
    std::shared_ptr<Expr> assignment();
    std::shared_ptr<Expr> or_expr();
    std::shared_ptr<Expr> and_expr();
    std::shared_ptr<Expr> equality();
    std::shared_ptr<Expr> comparison();
    std::shared_ptr<Expr> term();
    std::shared_ptr<Expr> factor();
    std::shared_ptr<Expr> unary();
    std::shared_ptr<Expr> call();
    std::shared_ptr<Expr> primary();

    std::shared_ptr<Expr> finish_call(std::shared_ptr<Expr> callee);

    std::string report_parse_error(const Token &token,
                                   const std::string &message);
    ParseError parse_error(const Token &token, const std::string &message);

    void synchronize();

    std::vector<Token> tokens;
    size_t curr;
    bool m_had_error;
};
