#pragma once

#include <string>

std::string error(int line, const std::string &message);

std::string report(int line, const std::string &where,
                   const std::string &message, bool print_message = true);
