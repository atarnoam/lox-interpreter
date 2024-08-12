#pragma once

#include <iostream>
#include <optional>
#include <string>
#include <variant>

enum TokenType {
    // Single-character tokens.
    LEFT_PAREN,
    RIGHT_PAREN,
    LEFT_BRACE,
    RIGHT_BRACE,
    COMMA,
    DOT,
    MINUS,
    PLUS,
    SEMICOLON,
    SLASH,
    STAR,

    // One or two character tokens.
    BANG,
    BANG_EQUAL,
    EQUAL,
    EQUAL_EQUAL,
    GREATER,
    GREATER_EQUAL,
    LESS,
    LESS_EQUAL,

    // Literals.
    IDENTIFIER,
    STRING,
    NUMBER,

    // Keywords.
    AND,
    CLASS,
    ELSE,
    FALSE,
    FUN,
    FOR,
    IF,
    NIL,
    OR,
    PRINT,
    RETURN,
    SUPER,
    THIS,
    TRUE,
    VAR,
    WHILE,
    END_OF_FILE
};

using TokenLiteral = std::variant<double, std::string>;

struct Token {
    Token(TokenType type, std::string lexeme, int line);
    Token(TokenType type, std::string lexeme, int line, double number);
    Token(TokenType type, std::string lexeme, int line, std::string literal);

    std::string literal_to_string() const;

    TokenType type;
    std::string lexeme;
    int line;
    std::optional<TokenLiteral> literal;
};

std::ostream &operator<<(std::ostream &os, const Token &token);