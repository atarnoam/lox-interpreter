#include "lox_instance.h"

LoxInstance::LoxInstance(const LoxClass *lclass) : lclass(lclass) {}

std::string LoxInstance::to_string() const {
    return lclass->to_string() + " instance";
}

std::ostream &operator<<(std::ostream &os, const LoxInstance &callable) {
    return os << callable.to_string();
}
