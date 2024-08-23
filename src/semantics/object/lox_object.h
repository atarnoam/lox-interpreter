#pragma once

#include "src/semantics/object/lox_callable.fwd.h"
#include "src/syntactics/token.h"
#include "src/tp_utils.h"

#include <compare>
#include <string>
#include <variant>
#include <vector>

struct LoxNull {
    operator bool() const;
    bool operator=(const LoxNull &other) const;
    bool operator!=(const LoxNull &other) const;
};

std::ostream &operator<<(std::ostream &os, const LoxNull &null);

struct LoxObject {
    using LoxObjectT = std::variant<LoxNull, bool, double, std::string,
                                    std::shared_ptr<LoxCallable>>;

    LoxObject() = default;
    LoxObject(LoxObjectT object);
    LoxObject(TokenLiteral token_literal);
    template <typename T>
    LoxObject(T x) : object(std::move(x)) {}

    operator LoxObjectT() const;

    template <class T>
    constexpr T &get() {
        return std::get<T>(object);
    }
    template <class T>
    constexpr const T &get() const {
        return std::get<T>(object);
    }

    template <class T>
    constexpr bool holds_alternative() const {
        return std::holds_alternative<T>(object);
    }

    explicit operator bool() const;

    bool operator==(const LoxObject &other) const;
    std::partial_ordering operator<=>(const LoxObject &other) const;

  private:
    LoxObjectT object;
};
