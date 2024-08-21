#pragma once

#include <concepts>
#include <memory>
#include <type_traits>

template <typename TypeToCheck, typename TypeToCheckAgainst>
concept type_is =
    std::same_as<std::remove_cvref_t<TypeToCheck>, TypeToCheckAgainst>;

template <typename S, typename... Args>
concept are_all_eq = (type_is<S, Args> && ...);

template <class... Args> struct variant_cast_proxy {
    std::variant<Args...> v;

    template <class... ToArgs> operator std::variant<ToArgs...>() const {
        return std::visit(
            [](auto &&arg) -> std::variant<ToArgs...> { return arg; }, v);
    }
};

template <class... Args>
auto variant_cast(const std::variant<Args...> &v)
    -> variant_cast_proxy<Args...> {
    return {v};
}

template <class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
template <class... Ts> overloaded(Ts...) -> overloaded<Ts...>;
