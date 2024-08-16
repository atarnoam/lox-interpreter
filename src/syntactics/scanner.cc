#include "src/logging.h"
#include "src/syntactics/scanner.h"

#include <iostream>
#include <sstream>
#include <unordered_map>

const std::unordered_map<std::string, TokenType> keywords{
    {"and", AND},   {"class", CLASS}, {"else", ELSE},     {"false", FALSE},
    {"for", FOR},   {"fun", FUN},     {"if", IF},         {"nil", NIL},
    {"or", OR},     {"print", PRINT}, {"return", RETURN}, {"super", SUPER},
    {"this", THIS}, {"true", TRUE},   {"var", VAR},       {"while", WHILE}};

Scanner::Scanner(std::istream &&is)
    : had_error(false), token_start_idx(0), curr_idx(0), line(1) {
    std::ostringstream ss;
    ss << is.rdbuf();
    source = ss.str();
}

Token &Scanner::add_token(TokenType type) {
    return tokens.emplace_back(type, curr_token(), line);
}
Token &Scanner::add_token(TokenType type, double number) {
    return tokens.emplace_back(
        type, source.substr(token_start_idx, curr_idx - token_start_idx), line,
        number);
}
Token &Scanner::add_token(TokenType type, std::string literal) {
    return tokens.emplace_back(
        type, source.substr(token_start_idx, curr_idx - token_start_idx), line,
        std::move(literal));
}

void Scanner::log_error(const std::string &message) {
    had_error = true;
    error(line, message);
}

bool Scanner::is_at_end() const { return curr_idx >= source.length(); }

std::string Scanner::curr_token() {
    return source.substr(token_start_idx, curr_idx - token_start_idx);
}

char Scanner::advance() { return source[curr_idx++]; }
bool Scanner::match(char expected) {
    if (is_at_end())
        return false;
    if (source[curr_idx] != expected)
        return false;

    curr_idx++;
    return true;
}

char Scanner::peek() const {
    if (is_at_end())
        return '\0';
    return source[curr_idx];
}
char Scanner::peek_next() const {
    if (curr_idx + 1 >= source.length())
        return '\0';
    return source[curr_idx + 1];
}

std::vector<Token> Scanner::scan_tokens() {
    while (!is_at_end()) {
        scan_token();
        token_start_idx = curr_idx;
    }
    add_token(END_OF_FILE);
    return tokens;
}

bool is_identifier_start(char c) { return isalpha(c) || c == '_'; }
bool is_identifier(char c) { return isalnum(c) || c == '_'; }

void Scanner::scan_token() {
    char c = advance();
    switch (c) {
    case '(':
        add_token(LEFT_PAREN);
        break;
    case ')':
        add_token(RIGHT_PAREN);
        break;
    case '{':
        add_token(LEFT_BRACE);
        break;
    case '}':
        add_token(RIGHT_BRACE);
        break;
    case ',':
        add_token(COMMA);
        break;
    case '.':
        add_token(DOT);
        break;
    case '-':
        add_token(MINUS);
        break;
    case '+':
        add_token(PLUS);
        break;
    case ';':
        add_token(SEMICOLON);
        break;
    case '*':
        add_token(STAR);
        break;
    case '!':
        add_token(match('=') ? BANG_EQUAL : BANG);
        break;
    case '=':
        add_token(match('=') ? EQUAL_EQUAL : EQUAL);
        break;
    case '<':
        add_token(match('=') ? LESS_EQUAL : LESS);
        break;
    case '>':
        add_token(match('=') ? GREATER_EQUAL : GREATER);
        break;
    case '/':
        if (match('/')) {
            // A comment goes until the end of the line.
            while (peek() != '\n' && !is_at_end())
                advance();
        } else {
            add_token(SLASH);
        }
        break;
    case '"':
        string();
        break;
    case ' ':
    case '\r':
    case '\t':
        break;
    case '\n':
        line++;
        break;
    default:
        if (isdigit(c)) {
            number();
        } else if (is_identifier_start(c)) {
            identifier();
        } else {
            log_error(std::string("Unexpected character: ") + c);
            had_error = true;
        }
    }
}

void Scanner::string() {
    while (peek() != '"' and !is_at_end()) {
        if (peek() == '\n')
            line++;
        advance();
    }
    if (is_at_end()) {
        log_error("Unterminated string.");
        return;
    }
    advance();
    add_token(STRING, source.substr(token_start_idx + 1,
                                    curr_idx - token_start_idx - 2));
}

void Scanner::number() {
    while (isdigit(peek()))
        advance();

    // Look for a fractional part.
    if (peek() == '.' && isdigit(peek_next())) {
        // Consume the "."
        advance();

        while (isdigit(peek()))
            advance();
    }

    add_token(NUMBER, std::stod(curr_token()));
}

void Scanner::identifier() {
    while (is_identifier(peek())) {
        advance();
    }

    auto type_it = keywords.find(curr_token());
    TokenType type = type_it == keywords.end() ? IDENTIFIER : type_it->second;
    add_token(type, curr_token());
}