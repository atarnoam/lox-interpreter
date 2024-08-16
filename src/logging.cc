#include "src/logging.h"

#include <fmt/format.h>
#include <iostream>

std::string error(int line, const std::string &message) {
    return report(line, "", message);
}

std::string report(int line, const std::string &where,
                   const std::string &message, bool print_message) {
    std::string full_message =
        fmt::format("[line {}] Error {}: {}", line, where, message);
    if (print_message) {
        std::cerr << full_message << std::endl;
    }
    return full_message;
}