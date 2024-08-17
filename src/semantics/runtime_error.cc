#include "src/semantics/runtime_error.h"

RuntimeError::RuntimeError(Token token, const std::string &what)
    : std::runtime_error(what), token(std::move(token)) {}
