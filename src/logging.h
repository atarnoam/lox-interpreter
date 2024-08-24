#pragma once

#include <string>

#include "src/syntactics/token.h"

std::string error(int line, const std::string &message);

std::string report(int line, const std::string &where,
                   const std::string &message, bool print_message = true);

std::string report_token_error(const Token &token, const std::string &message);
