#pragma once
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "src/token.h"

struct Scanner {
    explicit Scanner(std::istream &&is);

    Token &add_token(TokenType type);
    Token &add_token(TokenType type, double number);
    Token &add_token(TokenType type, std::string literal);
    std::vector<Token> scan_tokens();

    bool had_error;

  private:
    void log_error(const std::string &error);
    bool is_at_end() const;

    std::string curr_token();

    char advance();
    bool match(char expected);
    char peek() const;
    char peek_next() const;

    void scan_token();
    void string();
    void number();
    void identifier();

    std::string source;
    std::vector<Token> tokens;
    size_t token_start_idx;
    size_t curr_idx;
    size_t line;
};