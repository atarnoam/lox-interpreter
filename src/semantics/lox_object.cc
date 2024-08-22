#include "src/semantics/lox_object.h"
#include "lox_object.h"
// #include "src/semantics/print_lox_callable.h"
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
                          [](const std::shared_ptr<LoxCallable> &ptr) {
                              return ptr != nullptr;
                          },
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
