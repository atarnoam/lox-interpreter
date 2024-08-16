#include "src/syntactics/token.h"

#include <sstream>

Token::Token(TokenType type, std::string lexeme, int line)
    : type(type), lexeme(std::move(lexeme)), line(line) {}
Token::Token(TokenType type, std::string lexeme, int line, double number)
    : type(type), lexeme(std::move(lexeme)), line(line), literal(number) {}
Token::Token(TokenType type, std::string lexeme, int line, std::string literal)
    : type(type), lexeme(std::move(lexeme)), line(line),
      literal(std::move(literal)) {}

std::string Token::literal_to_string() const {
    if (literal.has_value()) {
        std::stringstream ss;
        std::visit([&ss](auto &x) { ss << x; }, literal.value());
        return ss.str();
    } else {
        return "";
    }
}

std::ostream &operator<<(std::ostream &os, const Token &token) {
    os << token.type << " " << token.lexeme;
    os << " " << token.literal_to_string();
    return os;
}