#include "print_lox_object.h"

#include "src/semantics/object/lox_callable.h"
#include "src/semantics/object/lox_instance.h"

std::ostream &operator<<(std::ostream &os, const LoxObject &lox_object) {
    return std::visit(
        overloaded{
            [&os](const std::shared_ptr<LoxCallable> &x) -> std::ostream & {
                return os << *x;
            },
            [&os](const std::shared_ptr<LoxInstance> &x) -> std::ostream & {
                return os << *x;
            },
            [&os](const auto &x) -> std::ostream & { return os << x; },
        },
        static_cast<LoxObject::LoxObjectT>(lox_object));
}
