#include "src/semantics/lox_object.h"

#include "src/tp_utils.h"

LoxNull::operator bool() const { return false; }

bool LoxNull::operator=(const LoxNull &other) const { return true; };
bool LoxNull::operator!=(const LoxNull &other) const { return false; };

std::ostream &operator<<(std::ostream &os, const LoxNull &null) {
    return os << "null";
}

LoxObject::LoxObject(LoxObjectT object) : object(std::move(object)) {}

LoxObject::LoxObject(TokenLiteral token_literal)
    : object(variant_cast(std::move(token_literal))) {}

LoxObject::operator LoxObjectT() const { return object; }

LoxObject::operator bool() const {
    return std::visit(overloaded{
                          [](const std::string &x) { return !x.empty(); },
                          [](auto x) -> bool { return x; },
                      },
                      object);
}

bool LoxObject::operator==(const LoxObject &other) const {
    return object == other.object;
}
std::partial_ordering LoxObject::operator<=>(const LoxObject &other) const {
    return get<double>() <=> other.get<double>();
}
#include <sstream>

std::ostream &operator<<(std::ostream &os, const LoxObject &lox_object) {
    return std::visit([&os](auto &&x) -> std::ostream & { return os << x; },
                      static_cast<LoxObject::LoxObjectT>(lox_object));
}
