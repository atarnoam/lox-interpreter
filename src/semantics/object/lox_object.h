#pragma once

#include "src/semantics/object/lox_callable.fwd.h"
#include "src/semantics/object/lox_instance.fwd.h"
#include "src/syntactics/token.h"
#include "src/tp_utils.h"

#include <compare>
#include <concepts>
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
    // TODO: Change shared_ptr to unique_ptr.
    using LoxObjectT = std::variant<LoxNull, bool, double, std::string,
                                    std::shared_ptr<LoxCallable>,
                                    std::shared_ptr<LoxInstance>>;

    LoxObject() = default;
    LoxObject(LoxObjectT object);
    LoxObject(TokenLiteral token_literal);
    template <typename T>
    LoxObject(T x) : object(std::move(x)) {}

    operator LoxObjectT() const;

    template <class T>
    T &get() {
        return std::get<T>(object);
    }
    template <class T>
    const T &get() const {
        return std::get<T>(object);
    }

    template <class T>
    requires std::derived_from<T, LoxCallable> std::shared_ptr<T> get() {
        auto lox_ptr = std::get<std::shared_ptr<LoxCallable>>(object);
        return std::dynamic_pointer_cast<T>(lox_ptr);
    }

    template <class T>
    requires std::derived_from<T, LoxCallable>
    const std::shared_ptr<T> &get() const {
        auto lox_ptr = std::get<std::shared_ptr<LoxCallable>>(object);
        return std::dynamic_pointer_cast<T>(lox_ptr);
    }

    template <class T>
    bool holds_alternative() const {
        return std::holds_alternative<T>(object);
    }

    template <class T>
    requires std::derived_from<T, LoxCallable>
    bool holds_alternative() const {
        using CallablePtr = std::shared_ptr<LoxCallable>;
        return std::holds_alternative<CallablePtr>(object) and
               (std::dynamic_pointer_cast<T>(std::get<CallablePtr>(object)) !=
                nullptr);
    }

    explicit operator bool() const;

    bool operator==(const LoxObject &other) const;
    std::partial_ordering operator<=>(const LoxObject &other) const;

  private:
    LoxObjectT object;
};
