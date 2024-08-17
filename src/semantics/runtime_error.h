#include "src/syntactics/token.h"
#include <stdexcept>

struct RuntimeError : std::runtime_error {
    explicit RuntimeError(Token token, const std::string &what);

    Token token;
};
