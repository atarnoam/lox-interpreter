#include "lox_callable.h"

std::ostream &operator<<(std::ostream &os, const LoxCallable &lox_callable) {
    return os << lox_callable.to_string();
}