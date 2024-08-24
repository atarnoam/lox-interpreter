#include "src/logging.h"

#include "logging.h"
#include <fmt/format.h>
#include <iostream>

std::string error(int line, const std::string &message) {
    return report(line, "", message);
}

std::string report(int line, const std::string &where,
                   const std::string &message, bool print_message) {
    std::string full_message =
        fmt::format("[line {}] Error{}{}: {}", line, where.empty() ? "" : " ",
                    where, message);
    if (print_message) {
        std::cerr << full_message << std::endl;
    }
    return full_message;
}

std::string report_token_error(const Token &token, const std::string &message) {
    if (token.type == END_OF_FILE) {
        return report(token.line, "at end", message);
    } else {
        return report(token.line, "at '" + token.lexeme + "'", message);
    }
}
